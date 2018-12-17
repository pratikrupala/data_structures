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

	// Lock file name size
	rwstats->super_block_size += sizeof(int);

	// Producer bitmap offset
	rwstats->producer_bitmap_offset = rwstats->super_block_size;

	// Producer bitmap size
	rwstats->super_block_size += (rwstats->buf_count/8) + 1;

	// Consumer bitmap offset
	rwstats->consumer_bitmap_offset = rwstats->super_block_size;

	// Consumer bitmap size
	rwstats->super_block_size += (rwstats->buf_count/8) + 1;

	rwstats->sblock_fd = shm_open(sblock_name, O_RDWR, 0666);
	printf("\nSBLOCK fd = %d\n", rwstats->sblock_fd);
	if (errno == ENOENT) {
		printf("\nSlock creating sh buf\n");
		rwstats->sblock_fd = shm_open(sblock_name,
				O_CREAT|O_RDWR, 0666);
	}
	if (rwstats->sblock_fd == -1) {
		printf("\nFailed to create shared memory for super block.\n");
		ret = -1;
		goto out;
	}

	if (ftruncate(rwstats->sblock_fd, rwstats->super_block_size)) {
		printf("\nSuperblock ftruncate failed.\n");
		return -1;
	}

	rwstats->sblock_lk_name = malloc((strlen(SBLOCK_LK_NAME)) * sizeof(char));
	if (!rwstats->sblock_lk_name) {
		printf("\nFailed to allocate memory for super block lock name"
				" string.\n");
		goto release_super_block;
	}

	snprintf(rwstats->sblock_lk_name, strlen(SBLOCK_LK_NAME), "%s", SBLOCK_LK_NAME);
	printf("\nDEBUG: sblock_lk_name = %s\n", rwstats->sblock_lk_name);

	rwstats->sblock_lk_fd = slock_mutex_create(rwstats->sblock_lk_name, &created);
	if (rwstats->sblock_lk_fd == -1) {
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
	goto out;
release_sblock_lk_name:
	free(rwstats->sblock_lk_name);
release_super_block:
	//TBD
out:
	return ret;
}
