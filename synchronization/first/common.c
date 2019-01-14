#include "write_helper.h"

void bitmap_set_bit(char *ptr, int bit)
{
	int *n = NULL;
	int loc = bit / 8;
	bit = bit % 8;

	ptr = ptr + loc;
	n = (int *) ptr;

	*n = *n | (1 << bit);
}

void bitmap_clear_bit(char *ptr, int bit)
{
	int *n = NULL;
	int loc = bit / 8;
	bit = bit % 8;

	ptr = ptr + loc;
	n = (int *) ptr;

	*n = *n & ~(1 << bit);
}

int bitmap_get_bit(char *ptr, int bit)
{
	int *n = NULL;
	int loc = bit / 8;
	bit = bit % 8;

	ptr = ptr + loc;
	n = (int *) ptr;

	return ((*n >> bit) & 1);
}

int slock_mutex_create(const char *sfile_name, int *created)
{
	int sfd = -1;
	void *mapped = NULL;

	pthread_mutex_t *slk;

	sfd = shm_open(sfile_name, O_RDWR, 0666);
	if (errno == ENOENT) {
		sfd = shm_open(sfile_name, O_CREAT|O_RDWR, 0666);
		if (created)
			*created = 1;
	} else {
		if (created)
			*created = 0;
		return sfd;
	}
	if (sfd == -1) {
		printf("\n%s: Failed to do shm_open.\n", __func__);
#ifdef DEBUG
		printf("\n%s: Error no = %d\n", __func__, errno);
		printf("\n%s: Error string = %s\n", __func__, strerror(errno));
#endif
		return -1;
	}

	if (ftruncate(sfd, sizeof(pthread_mutex_t))) {
		printf("\n%s: Ftruncate failed.\n", __func__);
		return -1;
	}

	mapped = mmap(NULL, sizeof(pthread_mutex_t), PROT_WRITE | PROT_READ,
			MAP_SHARED, sfd, 0);
	if (mapped == MAP_FAILED) {
		printf("\n%s: Failed to mmap the shared region.\n", __func__);
		return -1;
	}

	slk = (pthread_mutex_t *) mapped;

	pthread_mutexattr_t mattr;
	if (pthread_mutexattr_init(&mattr)) {
		printf("\n%s: Failed to initialize mutex attribute.\n",
				__func__);
		return -1;
	}

	if (pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED)) {
		printf("\n%s: Failed to set mutex attribute.\n", __func__);
		return -1;
	}

	if (pthread_mutex_init(slk, &mattr)) {
		printf("\n%s: Failed to initiliaze the shared mutex.\n",
			       __func__);
		return -1;
	}

	if (munmap(mapped, sizeof(pthread_mutex_t))) {
		printf("\n%s: Failed to unmap the shared mutex region.\n",
				__func__);
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
		printf("\n%s: Failed to allocate memory for slock_mapped.\n",
				__func__);
		goto out;
	}

	for (i = 0; i < count; i++) {
		slock_mapped[i] = (pthread_mutex_t *) mmap(NULL,
				sizeof(pthread_mutex_t),
				PROT_WRITE | PROT_READ, MAP_SHARED,
				slk_fds[i], 0);
		if (slock_mapped[i] == MAP_FAILED) {
			printf("\n%s: Failed to do mmap for destroying "
					"mutexes.\n", __func__);
			goto out;
		}
	}

	for (i = 0; i < count; i++) {
		ret = pthread_mutex_destroy(slock_mapped[i]);
		if (ret) {
			printf("\n%s: Failed to destroy mutex number %d.\n",
					__func__, i);
			goto out;
		}
	}
	ret = 0;

out:
	return ret;
}

int create_super_block(struct reader_writer_stats *rwstats)
{
	int ret = -1;
	int created = 1;
	char *sblock_name = SBLOCK_NAME;

	// Super block size itself
	rwstats->super_block_size += sizeof(int);

	// Buffer count size
	rwstats->super_block_size += sizeof(int);

	// Buffer file name size
	rwstats->super_block_size += sizeof(int);

	// Buffer name size
	rwstats->super_block_size += strlen(rwstats->buf_name) * sizeof(char);

	// Producer bitmap offset
	rwstats->producer_bitmap_offset = rwstats->super_block_size;

	// Producer bitmap size
	rwstats->super_block_size += (rwstats->buf_count/8) + 1;

	// Consumer bitmap offset
	rwstats->consumer_bitmap_offset = rwstats->super_block_size;

	// Consumer bitmap size
	rwstats->super_block_size += (rwstats->buf_count/8) + 1;

	rwstats->sblock_fd = shm_open(sblock_name, O_RDWR, 0666);
	if (errno == ENOENT) {
		rwstats->sblock_fd = shm_open(sblock_name,
				O_CREAT|O_RDWR, 0666);
	}
	if (rwstats->sblock_fd == -1) {
		printf("\n%s: Failed to create shared memory for super "
				"block.\n", __func__);
		ret = -1;
		goto out;
	}

	if (ftruncate(rwstats->sblock_fd, rwstats->super_block_size)) {
		printf("\n%s: Superblock ftruncate failed.\n", __func__);
		return -1;
	}

	rwstats->sblock_lk_name = malloc((strlen(SBLOCK_LK_NAME)) * sizeof(char));
	if (!rwstats->sblock_lk_name) {
		printf("\n%s: Failed to allocate memory for super block lock "
				"name string.\n", __func__);
		goto release_super_block;
	}

	snprintf(rwstats->sblock_lk_name, strlen(SBLOCK_LK_NAME), "%s", SBLOCK_LK_NAME);

	rwstats->sblock_lk_fd = slock_mutex_create(rwstats->sblock_lk_name, &created);
	if (rwstats->sblock_lk_fd == -1) {
		printf("\n%s: Failed to create shared memory buffer for keeping"
				" super block lock.\n", __func__);
		goto release_sblock_lk_name;
	}

	if (!created) {
		printf("\n%s: Super block lock already exists.\n", __func__);
		ret = 0;
		goto out;
	}
	ret = 0;
	goto out;
release_sblock_lk_name:
	free(rwstats->sblock_lk_name);
release_super_block:
	//TBD
out:
	return ret;
}

int create_shared_buffers_mappings(struct reader_writer_stats *rwstats)
{
	int i = 0;
	int buf_name_len = strlen(rwstats->buf_name) + 1;
	int ret = -1;
	int just_opened = 0;

	rwstats->buf_fds = malloc(rwstats->buf_count * sizeof(int));
	if (!rwstats->buf_fds) {
		printf("\n%s: Failed to allocaet memory for buffers fds.\n",
				__func__);
		goto out;
	}

	rwstats->buf_names = malloc(rwstats->buf_count * sizeof(char *));
	if (!rwstats->buf_names) {
		printf("\n%s: Failed to allocate memory for buffer name "
				"pointers.\n", __func__);
		goto release_buf_fds;
	}

	for (i = 0; i < rwstats->buf_count; i++)
	{
		rwstats->buf_names[i] = (char *) malloc(buf_name_len *
				sizeof(char));
		if (!rwstats->buf_names[i]) {
			printf("\n%s: Failed to allocate memory for buffer name"
					" for buffer number %d.\n", __func__, i);
			goto release_buf_pointers; 
		}
	}

	for (i = 0; i < rwstats->buf_count; i++) {
		sprintf(rwstats->buf_names[i], "%s%d", rwstats->buf_name, i);
	}

	for (i = 0; i < rwstats->buf_count; i++) {
		rwstats->buf_fds[i] = shm_open(rwstats->buf_names[i], O_RDWR,
				0660);
#ifdef DEBUG
		printf("\n%s: rwstats->buf_fds[%d] = %d, buf_count = %d\n",
				__func__, i, rwstats->buf_fds[i],
				rwstats->buf_count);
#endif
		if (errno == ENOENT) {
			rwstats->buf_fds[i] = shm_open(rwstats->buf_names[i],
					O_CREAT|O_RDWR, 0660);
		} else {
			just_opened = 1;
		}
		if (rwstats->buf_fds[i] == -1) {
			printf("\n%s: failed to create a shared buffer for "
					"buffer number %d\n", __func__, i);
			goto release_bufs;
		}
	}

	if (just_opened) {
		ret = 0;
		goto out;
	}

	for (i = 0; i < rwstats->buf_count; i++) {
		if (ftruncate(rwstats->buf_fds[i], BUF_SIZE)) {
			printf("\n%s: Failed to set size for shared buffer"
					" number %d\n", __func__, i);
			goto close_fds;
		}
	}
	ret = 0;
	goto out;

close_fds:
	for (i = 0; i < rwstats->buf_count; i++) {
		if (shm_unlink(rwstats->buf_names[i])) {
			printf("\n%s:Failed to destroy shared buffer"
					" number %d\n", __func__, i);
		}
	}
release_bufs:
	for (i = 0; i < rwstats->buf_count; i++) {
		free(rwstats->buf_names[i]);
	}
release_buf_pointers:
	free(rwstats->buf_names);
release_buf_fds:
	free(rwstats->buf_fds);
out:
	return ret;
}

void **open_all_shared_bufs(struct reader_writer_stats *rwstats)
{
	int i = 0;
	void **sbuf_mapped = NULL;

	sbuf_mapped = malloc(rwstats->buf_count * sizeof(void *));
	if (!sbuf_mapped) {
		printf("\n%s: Failed to allocate memory for sbuf_mapped.\n",
				__func__);
		goto out;
	}

	for (i = 0; i < rwstats->buf_count; i++) {
		sbuf_mapped[i] = mmap(NULL, BUF_SIZE,
				PROT_WRITE | PROT_READ, MAP_SHARED,
				rwstats->buf_fds[i], 0);
		if (sbuf_mapped[i] == MAP_FAILED) {
			printf("\n%s: Failed to mmap the shared buffer number "
					"%d for filling up block, buffer fd = "
					"%d.\n", __func__, i,
					rwstats->buf_fds[i]);
#ifdef DEBUG
			perror("Buffer mmap failed: ");
#endif
			goto out;
		}
	}

out:
	return sbuf_mapped;
}
