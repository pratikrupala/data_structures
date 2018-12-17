#include "common.h"

int read_super_block(struct reader_writer_stats *rwstats)
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
	rwstats->super_block_size = *ptr;
	printf("\nPRATIK: super_block_size = %d\n", rwstats->super_block_size);
	ptr++;
	rwstats->buf_count = *ptr;
	printf("\nPRATIK: buf_count = %d\n", rwstats->buf_count);
	ptr++;
	rwstats->buf_name_len = *ptr;
	ptr++;
	buf = (char *) ptr;
	rwstats->buf_name = malloc(rwstats->buf_name_len * sizeof(char));
	strncpy(rwstats->buf_name, buf, rwstats->buf_name_len - 1);
	rwstats->buf_name[rwstats->buf_name_len] = '\0';
	printf("\nPRATIK: buf_name = %s, len = %d\n", rwstats->buf_name,
			rwstats->buf_name_len);

	ret = 0;

out:
	return ret;
}
