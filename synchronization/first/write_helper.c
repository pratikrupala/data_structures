#include "write_helper.h"

int slock_mutex_create(const char *sfile_name, int *created)
{
	int sfd = -1;
	void *mapped = NULL;

	pthread_mutex_t *slk;

	sfd = shm_open(sfile_name, O_RDWR, 0666);
	printf("\nSLOCK sfd = %d\n", sfd);
	perror("SLOCK sfd");
	if (errno == ENOENT) {
		printf("\nSlock creating sh buf\n");
		sfd = shm_open(sfile_name, O_CREAT|O_RDWR, 0666);
		if (created)
			*created = 1;
	} else {
		if (created)
			*created = 0;
		return sfd;
	}
	if (sfd == -1) {
		printf("\nFailed to do shm_open.\n");
		printf("\nErr no = %d\n", errno);
		printf("\nErr str = %s\n", strerror(errno));
		return -1;
	}

	if (ftruncate(sfd, sizeof(pthread_mutex_t))) {
		printf("\nFtruncate failed.\n");
		return -1;
	}

	mapped = mmap(NULL, sizeof(pthread_mutex_t), PROT_WRITE | PROT_READ,
			MAP_SHARED, sfd, 0);
	if (mapped == MAP_FAILED) {
		printf("\nFailed to mmap the shared region.\n");
		return -1;
	}

	slk = (pthread_mutex_t *) mapped;

	pthread_mutexattr_t mattr;
	if (pthread_mutexattr_init(&mattr)) {
		printf("\nFailed to initialize mutex attribute.\n");
		return -1;
	}

	if (pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED)) {
		printf("\nFailed to set mutex attribute.\n");
		return -1;
	}

	if (pthread_mutex_init(slk, &mattr)) {
		printf("\nFailed to initiliaze the shared mutex.\n");
		return -1;
	}

	if (munmap(mapped, sizeof(pthread_mutex_t))) {
		printf("\nFailed to unmap the shared mutex region.\n");
		return -1;
	}

	return sfd;
}

int slock_mutex_delete(int *slk_fds, int count)
{
	int i = 0;
	int ret = -1;
	void **slock_mapped = NULL;

	slock_mapped = malloc(count * sizeof(void *));
	if (!slock_mapped) {
		printf("\nFailed to allocate memory for slock_mapped.\n");
		goto out;
	}

	for (i = 0; i < count; i++) {
		slock_mapped[i] = (pthread_mutex_t *) mmap(NULL,
				sizeof(pthread_mutex_t),
				PROT_WRITE | PROT_READ, MAP_SHARED,
				slk_fds[i], 0);
		if (slock_mapped[i] == MAP_FAILED) {
			printf("\nFailed to do mmap for destroying mutexes.\n");
			goto out;
		}
	}

	for (i = 0; i < count; i++) {
		ret = pthread_mutex_destroy(slock_mapped[i]);
		if (ret) {
			printf("\nFailed to destroy mutex number %d.\n", i);
			goto out;
		}
	}
	ret = 0;

out:
	return ret;
}

int create_super_block(struct writer_stats *wstats, char *file_name)
{
	int ret = -1;
	int created = 1;
	char *sblock_name = SBLOCK_NAME;

	// Super block size itself
	wstats->super_block_size += sizeof(int);

	// Buffer count size
	wstats->super_block_size += sizeof(int);

	// Buffer file name size
	wstats->super_block_size += sizeof(int);

	// Lock file name size
	wstats->super_block_size += sizeof(int);

	// Buffer file name
	wstats->super_block_size += strlen(file_name);

	// Lock file name
	wstats->super_block_size += strlen(wstats->buf_lk_name);

	// Producer bitmap offset
	wstats->producer_bitmap_offset = wstats->super_block_size;

	// Producer bitmap size
	wstats->super_block_size += (wstats->buf_count/8) + 1;

	// Consumer bitmap offset
	wstats->consumer_bitmap_offset = wstats->super_block_size;

	// Consumer bitmap size
	wstats->super_block_size += (wstats->buf_count/8) + 1;

	wstats->sblock_fd = shm_open(sblock_name, O_RDWR, 0666);
	printf("\nSBLOCK fd = %d\n", wstats->sblock_fd);
	if (errno == ENOENT) {
		printf("\nSlock creating sh buf\n");
		wstats->sblock_fd = shm_open(sblock_name,
				O_CREAT|O_RDWR, 0666);
	}
	if (wstats->sblock_fd == -1) {
		printf("\nFailed to create shared memory for super block.\n");
		ret = -1;
		goto out;
	}

	if (ftruncate(wstats->sblock_fd, wstats->super_block_size)) {
		printf("\nSuperblock ftruncate failed.\n");
		return -1;
	}

	wstats->sblock_lk_name = malloc((strlen(file_name)) * sizeof(char));
	if (!wstats->sblock_lk_name) {
		printf("\nFailed to allocate memory for super block lock name"
				" string.\n");
		goto release_super_block;
	}

	sprintf(wstats->sblock_lk_name, "sblock_%s", file_name);
	printf("\nDEBUG: sblock_lk_name = %s\n", wstats->sblock_lk_name);

	wstats->sblock_lk_fd = slock_mutex_create(wstats->sblock_lk_name, &created);
	if (wstats->sblock_lk_fd == -1) {
		printf("\nFailed to create shared memory buffer for keeping"
				" super block lock.\n");
		goto release_sblock_lk_name;
	}

	if (!created) {
		printf("\nSuper block lock already exists.\n");
		ret = 0;
		goto out;
	}

	ret = 0;
release_sblock_lk_name:
	free(wstats->sblock_lk_name);
release_super_block:
	//TBD
out:
	return ret;
}

int initialize_super_block(struct writer_stats *wstats)
{
	int ret = -1;
	void *sblock_mapped = NULL;
	int *ptr = NULL;
	char *buf = NULL;

	sblock_mapped = mmap(NULL, wstats->super_block_size,
			PROT_WRITE | PROT_READ, MAP_SHARED,
			wstats->sblock_fd, 0);
	if (sblock_mapped == MAP_FAILED) {
		printf("\nFailed to mmap the shared region of super block.\n");
		goto out;
	}

	ptr = (int *) sblock_mapped;
	*ptr = wstats->super_block_size;
	ptr++;
	*ptr = wstats->buf_count;
	ptr++;
	*ptr = wstats->buf_name_len;
	ptr++;
	*ptr = wstats->buf_lk_name_len;
	ptr++;
	buf = (char *) ptr;
	strncpy(buf, wstats->buf_name, strlen(wstats->buf_name));
	buf += strlen(wstats->buf_name);
	strncpy(buf, wstats->buf_lk_name, strlen(wstats->buf_lk_name));
	buf += strlen(wstats->buf_lk_name);
	memset(buf, 0, (wstats->buf_count/8) + 1);
	buf += (wstats->buf_count/8) + 1;
	memset(buf, 0, (wstats->buf_count/8) + 1);

	if (munmap(sblock_mapped, wstats->super_block_size)) {
		printf("\nFailed to unmap the shared superblock region.\n");
		goto out;
	}

	ret = 0;
out:
	return ret;
}

int create_lock_resources(struct writer_stats *wstats)
{
	int i = 0;
	//	void **slock_mapped = NULL;
	int ret = -1;

	printf ("writer started.\n");

	wstats->slock_fd = malloc(wstats->buf_count * sizeof(int));
	if (!wstats->slock_fd) {
		printf("\nFailed to allocate memory for fd of"
				"slock.\n");
		goto out;
	}

	wstats->lk_fname = malloc(wstats->buf_count * sizeof(char *));
	if (!wstats->lk_fname) {
		printf("\nFailed to allocate memory for lk_fname.\n");
		goto release_fds;
	}

	for (i = 0; i < wstats->buf_count; i++) {
		// TBD: for suffix 0 to 9, below is OK, but for further
		// increase, size might need to be updated.
		wstats->lk_fname[i] = malloc(strlen(wstats->buf_lk_name) + 1);
		if (!wstats->lk_fname) {
			printf("\nFailed to allocate memory for lk_name"
					" strings\n");
			goto release_fnames;
		}
	}

	for (i = 0; i < wstats->buf_count; i++) {
		sprintf(wstats->lk_fname[i], "lk_%s%d", wstats->buf_lk_name,
				i);
	}

	printf("\nFile names generated:\n");
	for (i = 0; i < wstats->buf_count; i++) {
		printf("%s\n", wstats->lk_fname[i]);
	}

	for (i = 0; i < wstats->buf_count; i++) {
		wstats->slock_fd[i] = slock_mutex_create(wstats->lk_fname[i], NULL);
		printf("\nWriter receiving sfd = %d\n", wstats->slock_fd[i]);
		if (wstats->slock_fd[i] == -1) {
			printf("\nWriter failed to create mutex for buffer"
					" number %d.\n", i + 1);
			goto release_fname_strings;
		}
	}

	/*
	   slock_mapped = malloc(buf_count * sizeof(void *));
	   if (!slock_mapped) {
	   printf("\nFailed to allocate memory for slock_mapped.\n");
	   goto destroy_mutexes;
	   }

	   for (i = 0; i < buf_count; i++) {
	   slock_mapped[i] = (pthread_mutex_t *) mmap(NULL,
	   sizeof(pthread_mutex_t),
	   PROT_WRITE | PROT_READ, MAP_SHARED,
	   wstats->slock_fd[i], 0);
	   if (slock_mapped[i] == MAP_FAILED) {
	   printf("\nFailed to do mmap for %s\n", wstats->lk_fname[i]);
	   goto release_mapped_pointer;
	   }
	   }

	   printf("\nCalling pthread_mutex_lock\n");
	   if (pthread_mutex_lock(slock_mapped[0])) {
	   printf("\nWriter failed to acquire lock.\n");
	   goto release_mapped_regions;
	   }
	   printf("\nLock acquired.\n");

	   sleep(10);

	   if (pthread_mutex_unlock(slock_mapped[0])) {
	   printf("\nWriter failed to release lock.\n");
	   goto release_mapped_regions;
	   }

	   for (i = 0; i < buf_count; i++) {
	   if (shm_unlink(lk_fname[i])) {
	   printf("\nFailed to unlink %s\n", lk_fname[i]);
	   goto release_mapped_regions;
	   }
	   }
	   */
	ret = 0;
	goto out;

	/*
release_mapped_regions:
for (i = 0; i < buf_count; i++) {
if (munmap(slock_mapped[i], sizeof(pthread_mutex_t))) {
printf("\nFailed to unmap the slock mapped"
"regions.\n");
}
}
release_mapped_pointer:
free(slock_mapped);
destroy_mutexes:
slock_mutex_delete(wstats->slock_fd, buf_count);
*/
release_fname_strings:
for (i = 0; i < wstats->buf_count; i++) {
	free(wstats->lk_fname[i]);
}
release_fnames:
free(wstats->lk_fname);
release_fds:
free(wstats->slock_fd);
out:
return ret;
}

int create_shared_buffers(struct writer_stats *wstats)
{
	int i = 0;
	int buf_name_len = strlen(wstats->buf_name) + 1;
	int ret = -1;

	wstats->buf_fds = malloc(wstats->buf_count * sizeof(int));
	if (!wstats->buf_fds) {
		printf("\nFailed to allocaet memory for buffers fds.\n");
		goto out;
	}

	wstats->buf_names = malloc(wstats->buf_count * sizeof(char *));
	if (!wstats->buf_names) {
		printf("\nFailed to allocate memory for buffer name pointers.\n");
		goto release_buf_fds;
	}

	for (i = 0; i < wstats->buf_count; i++)
	{
		wstats->buf_names[i] = (char *) malloc(buf_name_len *
				sizeof(char));
		if (!wstats->buf_names[i]) {
			printf("\nFailed to allocate memory for buffer name"
					" for buffer number %d.\n", i);
			goto release_buf_pointers; 
		}
	}

	for (i = 0; i < wstats->buf_count; i++) {
		sprintf(wstats->buf_names[i], "%s%d", wstats->buf_name, i);
	}

	for (i = 0; i < wstats->buf_count; i++) {
		wstats->buf_fds[i] = shm_open(wstats->buf_names[i], O_RDWR, 0660);
		if (errno == ENOENT) {
			wstats->buf_fds[i] = shm_open(wstats->buf_names[i],
					O_CREAT|O_RDWR, 0660);
		}
		if (wstats->buf_fds[i] == -1) {
			printf("\nfailed to create a shared buffer for buffer"
					" number %d\n", i);
			goto release_bufs;
		}
	}

	for (i = 0; i < wstats->buf_count; i++) {
		if (ftruncate(wstats->buf_fds[i], BUF_SIZE)) {
			printf("\nFailed to set size for shared buffer"
					" number %d\n", i);
			goto close_fds;
		}
	}
	ret = 0;
	goto out;

close_fds:
	for (i = 0; i < wstats->buf_count; i++) {
		if (shm_unlink(wstats->buf_names[i])) {
			printf("\nFailed to destroy shared buffer"
					" number %d\n", i);
		}
	}
release_bufs:
	for (i = 0; i < wstats->buf_count; i++) {
		free(wstats->buf_names[i]);
	}
release_buf_pointers:
	free(wstats->buf_names);
release_buf_fds:
	free(wstats->buf_fds);
out:
	return ret;
}

int calculate_digits(int size)
{
	int digits = 0;

	while (size != 0) {
		size = size / 10;
		digits++;
	}

	return digits;
}

void bitmap_set_bit(char *ptr, int bit)
{
	int *n = NULL;
	int loc = bit / 8;

	ptr = ptr + loc;
	n = (int *) ptr;

	*n = *n | (1 << bit);
}

void bitmap_clear_bit(char *ptr, int bit)
{
	int *n = NULL;
	int loc = bit / 8;

	ptr = ptr + loc;
	n = (int *) ptr;

	*n = *n & ~(1 << bit);
}

int bitmap_get_bit(char *ptr, int bit)
{
	int *n = NULL;
	int loc = bit / 8;

	ptr = ptr + loc;
	n = (int *) ptr;

	return ((*n >> bit) & 1);
}

int confirm_buf_num(struct writer_stats *wstats, int *current_buf_size,
		int len_to_write, void *sblock_lk, char *producer_bitmap_ptr,
		char *consumer_bitmap_ptr, int *current_buf_num,
		void **sbuf_mapped, void **current_buf_ptr)
{
	int ret = -1;
	int original_buf_num = *current_buf_num;

	while ((*current_buf_size - 1) < len_to_write) {
		if (pthread_mutex_lock(sblock_lk)) {
			printf("\nFailed to acquire superblock lock\n");
			goto out;
		}
		// Set 1 in consumer bitmap for the current buffer
		bitmap_set_bit(consumer_bitmap_ptr, *current_buf_num);
		bitmap_clear_bit(producer_bitmap_ptr, *current_buf_num);

fetch_next_buf:
		// Need to move to next buffer
		*current_buf_num = (*current_buf_num + 1) % wstats->buf_count;
		if ((bitmap_get_bit(producer_bitmap_ptr, *current_buf_num)) ||
			(bitmap_get_bit(consumer_bitmap_ptr, *current_buf_num))) {
			goto fetch_next_buf;
		} else if (original_buf_num == *current_buf_num) {
			sleep(10);
			goto fetch_next_buf;
		} else {
			*current_buf_size = BUF_SIZE;
			*current_buf_ptr = sbuf_mapped[*current_buf_num];
			bitmap_set_bit(producer_bitmap_ptr, *current_buf_num);
		}
		if (pthread_mutex_unlock(sblock_lk)) {
			printf("\nFailed to release superblock lock\n");
			goto out;
		}
	}
	ret = 0;

out:
	return ret;
}

void **open_all_shared_bufs(struct writer_stats *wstats)
{
	int i = 0;
	void **sbuf_mapped = NULL;

	sbuf_mapped = malloc(wstats->buf_count * sizeof(void *));
	if (!sbuf_mapped) {
		printf("\nFailed to allocate memory for sbuf_mapped.\n");
		goto out;
	}

	for (i = 0; i < wstats->buf_count; i++) {
		sbuf_mapped[i] = mmap(NULL, BUF_SIZE,
				PROT_WRITE | PROT_READ, MAP_SHARED,
				wstats->buf_fds[i], 0);
		if (sbuf_mapped[i] == MAP_FAILED) {
			printf("\nFailed to mmap the shared buffer number %d "
					"for filling up block, buffer "
					"fd = %d.\n", i, wstats->buf_fds[i]);
			perror("Buffer mmap failed: ");
			goto out;
		}
	}

out:
	return sbuf_mapped;
}

int fill_shared_buffers(struct writer_stats *wstats, char *file_name)
{
	int j = 0;
	int ret = -1;
	int fd = -1;
	int start_count = 0;
	char buf[MAX_SIZE + 1];
	ssize_t c = 0;
	char *str = NULL;
	char *start = NULL;
	char *prev_buf = NULL;
	int prev_count = -1;
	char *cat_buf = NULL;
	int concatenate_len = -1;
	void **sbuf_mapped = NULL;
	int current_buf_num = 0;
	int current_buf_available_size = MAX_SIZE;
	int len_size = 0;
	void *sblock_lk = NULL;
	void *sblock_mapped = NULL;
	char *producer_bitmap_ptr = NULL;
	char *consumer_bitmap_ptr = NULL;
	char *size_info = NULL;
	char *data_line = NULL;
	int data_line_len = 0;
	int size_info_len = 0;
	void *current_buf_ptr = NULL;

	fd = open(file_name, O_RDONLY);
	if (fd == -1) {
		perror("Open failed:");
		goto out;
	}

	sblock_mapped = mmap(NULL, wstats->super_block_size,
			PROT_WRITE | PROT_READ, MAP_SHARED,
			wstats->sblock_fd, 0);
	if (sblock_mapped == MAP_FAILED) {
		printf("\nFailed to mmap the shared region of super block.\n");
		goto out;
	}

	sblock_lk = (pthread_mutex_t *) mmap(NULL, sizeof(pthread_mutex_t),
			PROT_WRITE | PROT_READ, MAP_SHARED,
			wstats->sblock_lk_fd, 0);
	if (sblock_lk == MAP_FAILED) {
		printf("\nFailed to mmap the superblock lock.\n");
		goto out;
	}

	producer_bitmap_ptr = (char *) sblock_mapped +
		wstats->producer_bitmap_offset;
	consumer_bitmap_ptr = (char *) sblock_mapped +
		wstats->consumer_bitmap_offset;

	sbuf_mapped = open_all_shared_bufs(wstats);
	if (!sbuf_mapped) {
		printf("\nFailed to open all bufs for writing.\n");
		goto out;
	}

	current_buf_ptr = sbuf_mapped[0];

	while (1) {
		static int i = 0;

		c = read(fd, (void *) buf, MAX_SIZE);
		if (c == 0) {
			printf("\nEOF\n");
			break;
		} else if (c == -1) {
			printf("\nError reading the file.\n");
			goto out;
		} else {
			// printf("\nRead %lu bytes.\n", c);
		}

		buf[MAX_SIZE] = '\0';

		for (j = 0, str = buf, start = buf, start_count = 0;
				j < MAX_SIZE && *start != '\0'; j++) {
			if ((*(str + j)) == '.') {
				/* 
				 * Replace '.' with '\0', so that printf can
				 * print it till '\0' only. Otherwise printf
				 * will keep printing utill it finds '\0'
				 * instead of printing the sentence till '.'
				 */
				(*(str + j)) = '\0';
				if (!prev_buf) {
					size_info_len = calculate_digits(strlen(start)) + 1;
					data_line_len = strlen(start) + 1;
					len_size = size_info_len + data_line_len;
					confirm_buf_num(wstats, &current_buf_available_size,
							len_size, sblock_lk, producer_bitmap_ptr,
							consumer_bitmap_ptr, &current_buf_num,
							sbuf_mapped, &current_buf_ptr);
					size_info = malloc(size_info_len * sizeof(char));
					snprintf(size_info, size_info_len, "%lu\n", strlen(start));
					strncpy(current_buf_ptr, size_info, size_info_len);
					current_buf_available_size -= size_info_len;
					current_buf_ptr += size_info_len;
					data_line = malloc(data_line_len * sizeof(char));
					snprintf(data_line, data_line_len, "%s\n", start);
					strncpy(current_buf_ptr, data_line, data_line_len);
					current_buf_available_size -= data_line_len;
					current_buf_ptr += data_line_len;
					printf("\nWriting to buf number = %d\n", current_buf_num);
					printf("\n[%d]: %s.\n", i++, start);
					free(size_info);
					free(data_line);
				} else {
					size_info_len = calculate_digits(strlen(start) +
							strlen(prev_buf)) + 1;
					data_line_len = strlen(prev_buf) + strlen(start);
					concatenate_len = data_line_len + 1;
					cat_buf = malloc(concatenate_len *
							sizeof(char));
					memset(cat_buf, '\0', strlen(cat_buf));
					if (!cat_buf) {
						printf("\nFailed to allocate memory for "
								"concatenation\n");
					}
					cat_buf = strncpy(cat_buf, prev_buf,
							strlen(prev_buf));
					*(cat_buf + strlen(prev_buf)) = '\0';
					cat_buf = strncat(cat_buf, start, concatenate_len);
					len_size = size_info_len + concatenate_len;
					confirm_buf_num(wstats, &current_buf_available_size,
							len_size, sblock_lk, producer_bitmap_ptr,
							consumer_bitmap_ptr, &current_buf_num,
							sbuf_mapped, &current_buf_ptr);
					size_info = malloc(size_info_len * sizeof(char));
					snprintf(size_info, size_info_len, "%lu\n", strlen(start) +
							strlen(prev_buf));
					strncpy(current_buf_ptr, size_info, size_info_len);
					current_buf_available_size -= size_info_len;
					current_buf_ptr += size_info_len;
					data_line = malloc(data_line_len * sizeof(char));
					snprintf(data_line, data_line_len, "%s\n", cat_buf);
					strncpy(current_buf_ptr, data_line, data_line_len);
					current_buf_available_size -= data_line_len;
					current_buf_ptr += data_line_len;
					printf("\nWriting to buf number = %d\n", current_buf_num);
					printf("\n[%d]: %s.\n", i++, cat_buf);
					if (prev_buf)
						free(prev_buf);
					if (cat_buf)
						free(cat_buf);
					prev_buf = NULL;
					cat_buf = NULL;
				}
				start_count = j + 1;
				start = buf + start_count;
			} else if (((j != 0) && ((*(str + (j - 1))) == '\0')
						&& ((*(str + j)) == '\n')) || ((j == 0)
						&& ((*(str + j) == '\n')))) {
				/*
				 * If previous sentence is already printed and
				 * '\0' is already placed, then '\n' at the
				 * start of the new sentence is not required.
				 * So just move forward by skipping leading
				 * '\n' of the current sentence. Also, if there
				 * is any leading '\n' in the sentence, remove
				 * the same.
				 */
				start += 1;
				start_count++;
			} else if ((*(str + j)) == '\n') {
				/*
				 * If '\n' is just between the current sentence
				 * then just replace it with space.
				 */
				(*(str + j)) = ' ';
			}
		}
		if (start_count < MAX_SIZE && *start != '\0') {
			/*
			 * If the sentence is incomplete at the end of the
			 * current buffer, save it before read() overwrites
			 * the current buffer content.
			 */
			prev_count = j - start_count + 1;
			if (prev_count < 1) {
				prev_buf = NULL;
				continue;
			}
			if (prev_buf) {
				prev_buf = realloc(prev_buf, strlen(prev_buf)
						+ prev_count);
				if (!prev_buf) {
					printf("\nFailed to realloc for "
							"previous buffer.\n");
					goto out;
				}
				prev_buf = strncat(prev_buf, start,
						strlen(prev_buf) +
						prev_count);
			} else {
				prev_buf = malloc(prev_count * sizeof(char));
				prev_buf = strncpy(prev_buf, start,
						prev_count);
				*(prev_buf + prev_count) = '\0';
				if (!prev_buf) {
					printf("\nThe strncpy() failed for"
							" incomplete "
							"sentence\n");
					goto out;
				}
			}
		}
	}

	ret = 0;
out:
	return ret;
}
