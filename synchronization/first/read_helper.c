#include <math.h>
#include "common.h"
#include "read_helper.h"

int map_super_block(struct reader_writer_stats *rwstats)
{
        int ret = -1;
	void *sblock_mapped = NULL;

	sblock_mapped = mmap(NULL, rwstats->super_block_size,
			PROT_WRITE | PROT_READ, MAP_SHARED,
			rwstats->sblock_fd, 0);
		       
	if (sblock_mapped == MAP_FAILED) {
		printf("\nFailed to mmap the shared region of super block.\n");
		goto out;
	}
	ret = 0;

out:
	return ret;
}


int get_length(char *buf, int buf_len)
{
	int i = 0;
	int j = 0;
	int len = 0;

	for (i = buf_len - 1; i >= 0; i--) {
//		printf("PRATIK: Reading character %c\n", buf[i]);
		len += (buf[i] - '0') * pow(10, j++);
	}

	return len;
}

int get_buf_num(struct reader_writer_stats *rwstats, void *sblock_lk,
		char *consumer_bitmap_ptr, int *current_buf_num)
{
	int ret = -1;

	if (*current_buf_num >= 0 && *current_buf_num < rwstats->buf_count) {
		if (pthread_mutex_lock(sblock_lk)) {
			printf("\nFailed to acquire superblock lock\n");
			goto out;
		}
		if (bitmap_get_bit(consumer_bitmap_ptr, *current_buf_num)) {
			bitmap_clear_bit(consumer_bitmap_ptr, *current_buf_num);
		}
		if (pthread_mutex_unlock(sblock_lk)) {
			printf("\nFailed to release superblock lock\n");
			goto out;
		}
	}

	// Need to move to next buffer
	*current_buf_num = (*current_buf_num + 1) % rwstats->buf_count;

fetch_next_buf:
	if (pthread_mutex_lock(sblock_lk)) {
		printf("\nFailed to acquire superblock lock\n");
		goto out;
	}
	if (!bitmap_get_bit(consumer_bitmap_ptr, *current_buf_num)) {
		if (pthread_mutex_unlock(sblock_lk)) {
			printf("\nFailed to release superblock lock\n");
			goto out;
		}
		printf("\nPRATIK: Going for sleep for buffer %d\n",
				*current_buf_num);
		sleep(2);
		goto fetch_next_buf;
	}
	if (pthread_mutex_unlock(sblock_lk)) {
		printf("\nFailed to release superblock lock\n");
		goto out;
	}
	printf("PRATIK: New buffer num = %d\n", *current_buf_num);
	ret = 0;
out:
	return ret;
}

int consume_shared_buffers(struct reader_writer_stats *rwstats,
		char *sub_string)
{
	int ret = -1;
	int fd = -1;
	void *sblock_mapped = NULL;
	void *sblock_lk = NULL;
	char *consumer_bitmap_ptr = NULL;
	void **sbuf_mapped = NULL;
	void *current_buf_ptr = NULL;
	int data_line_len = 0;
	char *ptr = NULL;
	char *start = NULL;
	char *len_buf = NULL;
	char *data_buf = NULL;
	int len_start = 0;
	int len_end = 0;
	int read_len = 0;
	int current_buf_num = -1;

	fd = open(OUTPUT_FILE, O_CREAT | O_WRONLY | O_TRUNC);
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

	consumer_bitmap_ptr = (char *) sblock_mapped +
		rwstats->consumer_bitmap_offset;

	sbuf_mapped = open_all_shared_bufs(rwstats);
	if (!sbuf_mapped) {
		printf("\nFailed to open all bufs for writing.\n");
		goto out;
	}

consume_next_buffer:
	get_buf_num(rwstats, sblock_lk, consumer_bitmap_ptr, &current_buf_num);
	current_buf_ptr = sbuf_mapped[current_buf_num];

	len_start = len_end = 0;
	ptr = (char *) current_buf_ptr;
	while (*ptr != '\0') {
		len_buf = data_buf = NULL;
		read_len = 0;
		len_start = len_end;
//		if (*ptr == '\n')
//			ptr++;
		start = ptr;
		data_line_len = 0;
		while (*ptr != '\n') {
			len_end++;
			ptr++;
		}
		read_len = len_end - len_start;
		printf("PRATIK: (%d - %d) = read_len = %d\n", len_end,
				len_start, read_len);
		len_buf = malloc(read_len * sizeof(char));
//		memset(len_buf, '\0', read_len);
		memcpy(len_buf, start, read_len);
		data_line_len = get_length(len_buf, read_len);
		printf("PRATIK: Length = %d\n", data_line_len);
		len_end++;
		ptr++;
		start = ptr;
		len_start = len_end;
		data_buf = malloc((data_line_len + 2) * sizeof(char));
//		memset(data_buf, '\0', (data_line_len + 2));
		data_buf[data_line_len] = '.';
		data_buf[data_line_len + 1] = '\n';
		memcpy(data_buf, start, data_line_len);
		len_end += data_line_len + 1;
		printf("PRATIK: Writing %s", data_buf);
		ret = write(fd, data_buf, data_line_len + 2);
		if (ret == -1) {
			perror("Write error:");
			goto out;
		}
		ptr += data_line_len + 1;
		if (*ptr == '\0') {
			printf("PRATIK: Ohh WOW! NULL ptr\n");
//			goto out;
		}
		free(len_buf);
		free(data_buf);
	}
	goto consume_next_buffer;
	ret = 0;
out:
	return ret;
}
