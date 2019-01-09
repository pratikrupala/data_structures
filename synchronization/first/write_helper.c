#include "write_helper.h"

int initialize_super_block(struct reader_writer_stats *rwstats)
{
	int ret = -1;
	void *sblock_mapped = NULL;
	int *ptr = NULL;
	char *buf = NULL;

	sblock_mapped = mmap(NULL, rwstats->super_block_size,
			PROT_WRITE | PROT_READ, MAP_SHARED,
			rwstats->sblock_fd, 0);
	if (sblock_mapped == MAP_FAILED) {
		printf("\nFailed to mmap the shared region of super block.\n");
		goto out;
	}

	ptr = (int *) sblock_mapped;
	*ptr = rwstats->super_block_size;
	ptr++;
	*ptr = rwstats->buf_count;
	ptr++;
	*ptr = rwstats->buf_name_len;
	ptr++;
	buf = (char *) ptr;
	strncpy(buf, rwstats->buf_name, strlen(rwstats->buf_name));
	buf += strlen(rwstats->buf_name);
	memset(buf, 0, (rwstats->buf_count/8) + 1);
	buf += (rwstats->buf_count/8) + 1;
	memset(buf, 0, (rwstats->buf_count/8) + 1);

	if (munmap(sblock_mapped, rwstats->super_block_size)) {
		printf("\nFailed to unmap the shared superblock region.\n");
		goto out;
	}

	ret = 0;
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

int confirm_buf_num(struct reader_writer_stats *rwstats, int *current_buf_size,
		int len_to_write, void *sblock_lk, char *producer_bitmap_ptr,
		char *consumer_bitmap_ptr, int *current_buf_num,
		void **sbuf_mapped, void **current_buf_ptr)
{
	int ret = -1;
	int original_buf_num = *current_buf_num;
	char *ptr = NULL;

	if ((*current_buf_size - 1) >= len_to_write) {
		if (bitmap_get_bit(consumer_bitmap_ptr, *current_buf_num)) {
			printf("\nAlready writing in the wrong buffer. "
					"Abort.\n");
			goto out;
		}
		if (!bitmap_get_bit(producer_bitmap_ptr, *current_buf_num)) {
			bitmap_set_bit(producer_bitmap_ptr, *current_buf_num);
		}
		ret = 0;
		goto out;
	}

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
		*current_buf_num = (*current_buf_num + 1) % rwstats->buf_count;
		if ((bitmap_get_bit(producer_bitmap_ptr, *current_buf_num)) ||
			(bitmap_get_bit(consumer_bitmap_ptr, *current_buf_num))) {
			exit(1);
//			sleep(2);
			/* 
			 * Either consumer has not consumed this buffer yet or
			 * other producer thread is writing to this buffer right
			 * now or somthing has gone wrong. so try some other buffer
			 * after sleeping for some time. Don't hold lock while
			 * going for sleep.
			 */	
			if (pthread_mutex_unlock(sblock_lk)) {
				printf("\nFailed to release superblock lock\n");
				goto out;
			}
			sleep(3);
			if (pthread_mutex_lock(sblock_lk)) {
				printf("\nFailed to acquire superblock lock\n");
				goto out;
			}
			goto fetch_next_buf;
		} else if (original_buf_num == *current_buf_num) {
			/*
			 * Reached at the same buffer from where we started, which
			 * means either consumer has not started consuming the buffers
			 * or somehow consumer is blocked. So before trying the same
			 * again, lets consumer give some time to consume or unblock
			 * itself and then resume again. Don't hold lock while
			 * going for sleep.
			 */
			if (pthread_mutex_unlock(sblock_lk)) {
				printf("\nFailed to release superblock lock\n");
				goto out;
			}
			sleep(3);
			if (pthread_mutex_lock(sblock_lk)) {
				printf("\nFailed to acquire superblock lock\n");
				goto out;
			}
			goto fetch_next_buf;
		} else {
			// We can use newly calculated buffer number
			*current_buf_size = BUF_SIZE;
			*current_buf_ptr = sbuf_mapped[*current_buf_num];
			ptr = (char *) *current_buf_ptr;
			ptr += BUF_SIZE;
			*ptr = '\0';
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

int fill_shared_buffers(struct reader_writer_stats *rwstats, char *file_name)
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
	char *ptr = NULL;

	fd = open(file_name, O_RDONLY);
	if (fd == -1) {
		perror("Open failed:");
		goto out;
	}

	sblock_mapped = mmap(NULL, rwstats->super_block_size,
			PROT_WRITE | PROT_READ, MAP_SHARED,
			rwstats->sblock_fd, 0);
	if (sblock_mapped == MAP_FAILED) {
		printf("\nFailed to mmap the shared region of super block.\n");
		goto out;
	}

	sblock_lk = (pthread_mutex_t *) mmap(NULL, sizeof(pthread_mutex_t),
			PROT_WRITE | PROT_READ, MAP_SHARED,
			rwstats->sblock_lk_fd, 0);
	if (sblock_lk == MAP_FAILED) {
		printf("\nFailed to mmap the superblock lock.\n");
		goto out;
	}

	producer_bitmap_ptr = (char *) sblock_mapped +
		rwstats->producer_bitmap_offset;
	consumer_bitmap_ptr = (char *) sblock_mapped +
		rwstats->consumer_bitmap_offset;

	sbuf_mapped = open_all_shared_bufs(rwstats);
	if (!sbuf_mapped) {
		printf("\nFailed to open all bufs for writing.\n");
		goto out;
	}

	current_buf_ptr = sbuf_mapped[0];
	ptr = (char *) current_buf_ptr;
	ptr += BUF_SIZE;
	*ptr = '\0';

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
					confirm_buf_num(rwstats, &current_buf_available_size,
							len_size, sblock_lk, producer_bitmap_ptr,
							consumer_bitmap_ptr, &current_buf_num,
							sbuf_mapped, &current_buf_ptr);
					size_info = malloc(size_info_len * sizeof(char));
					snprintf(size_info, size_info_len, "%lu\n", strlen(start));
					printf("PRATIK: Size info = %s, len = %d", size_info, size_info_len);
					strncpy(current_buf_ptr, size_info, size_info_len);
					current_buf_available_size -= size_info_len;
					current_buf_ptr += calculate_digits(strlen(start));
					ptr = (char *) current_buf_ptr;
					*ptr = '\n';
					current_buf_ptr++;
					data_line = malloc(data_line_len * sizeof(char));
					snprintf(data_line, data_line_len, "%s\n", start);
					strncpy(current_buf_ptr, data_line, data_line_len);
					current_buf_available_size -= data_line_len;
					current_buf_ptr += data_line_len - 1;
					ptr = (char *) current_buf_ptr;
					*ptr = '\n';
					current_buf_ptr++;
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
					confirm_buf_num(rwstats, &current_buf_available_size,
							len_size, sblock_lk, producer_bitmap_ptr,
							consumer_bitmap_ptr, &current_buf_num,
							sbuf_mapped, &current_buf_ptr);
					ptr = (char *) current_buf_ptr;
					size_info = malloc(size_info_len * sizeof(char));
					snprintf(size_info, size_info_len, "%lu\n", strlen(start) +
							strlen(prev_buf));
					strncpy(current_buf_ptr, size_info, size_info_len);
					current_buf_available_size -= size_info_len;
					current_buf_ptr += calculate_digits(strlen(start) +
							strlen(prev_buf));
					ptr = (char *) current_buf_ptr;
					*ptr = '\n';
					current_buf_ptr++;
					current_buf_ptr += size_info_len;
					data_line = malloc(data_line_len * sizeof(char));
					snprintf(data_line, data_line_len, "%s\n", cat_buf);
					strncpy(current_buf_ptr, data_line, data_line_len);
					current_buf_available_size -= data_line_len;
					current_buf_ptr += data_line_len - 1;
					ptr = (char *) current_buf_ptr;
					*ptr = '\n';
					current_buf_ptr++;
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
