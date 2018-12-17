#include "common.h"

int read_super_block(struct reader_writer_stats *rwstats)
{
        int ret = -1;
	void *sblock_mapped = NULL;
	int *ptr = NULL;

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

	ret = 0;

out:
	return ret;
}
