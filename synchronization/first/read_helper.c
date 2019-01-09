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
	char *ptr = NULL;
	char *current_size = 0;
	int size = 0;
	int j = 0;

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
	current_buf_ptr = sbuf_mapped[0];

	while (1) {
		int k = 0;
		j = 0;
		size = 0;
		ptr = (char *) current_buf_ptr;
		while (*ptr != '\n') {
			printf("%c", *ptr);
			j++;
			ptr++;
		}
		current_size = malloc(j * sizeof(char));
		printf("PRATIK: Before while, j = %d\n", j);
		memcpy((void *) current_size, current_buf_ptr, j);
		for (k = 0; k < j; k++) {
			size += pow(10, k) * *(current_size + k);
		}
		printf("size = %d\n", size);
		break;
	}
	ret = 0;
out:
	return ret;
}
