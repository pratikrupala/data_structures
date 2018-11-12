#include "write_helper.h"

int slock_mutex_create(const char *sfile_name, int *created)
{
	int sfd = -1;
	void *mapped = NULL;

	pthread_mutex_t *slk;

	sfd = shm_open(sfile_name, O_RDWR, 0666);
	printf("\nSLOCK sfd = %d\n", sfd);
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

	mapped = mmap(NULL, sizeof(pthread_mutex_t), PROT_WRITE | PROT_READ, MAP_SHARED, sfd, 0);
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

int create_super_block(struct writer_stats *wstats, int buf_count, char *file_name)
{
	int ret = -1;
	int created = 1;

	wstats->sblock_lk_name = malloc((strlen(file_name)) * sizeof(char));
	if (!wstats->sblock_lk_name) {
		printf("\nFailed to allocate memory for super block lock name"
				" string.\n");
		goto out;
	}

	wstats->sblock_fd = slock_mutex_create(wstats->sblock_lk_name, &created);
	if (wstats->sblock_fd == -1) {
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
out:
	return ret;
}

int create_lock_resources(struct writer_stats *wstats, int buf_count, char *file_name)
{
	int i = 0;
//	void **slock_mapped = NULL;
	int ret = -1;

	printf ("writer started.\n");

	wstats->slock_fd = malloc(buf_count * sizeof(int));
	if (!wstats->slock_fd) {
		printf("\nFailed to allocate memory for fd of"
				"slock.\n");
		goto out;
	}

	wstats->lk_fname = malloc(buf_count * sizeof(char *));
	if (!wstats->lk_fname) {
		printf("\nFailed to allocate memory for lk_fname.\n");
		goto release_fds;
	}

	for (i = 0; i < buf_count; i++) {
		// TBD: for suffix 0 to 9, below is OK, but for further
		// increase, size might need to be updated.
		wstats->lk_fname[i] = malloc(strlen(file_name) + 1);
		if (!wstats->lk_fname) {
			printf("\nFailed to allocate memory for lk_name"
					" strings\n");
			goto release_fnames;
		}
	}

	for (i = 0; i < buf_count; i++) {
		sprintf(wstats->lk_fname[i], "lk_%s%d", file_name, i);
	}

	printf("\nFile names generated:\n");
	for (i = 0; i < buf_count; i++) {
		printf("%s\n", wstats->lk_fname[i]);
	}

	for (i = 0; i < buf_count; i++) {
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
	for (i = 0; i < buf_count; i++) {
		free(wstats->lk_fname[i]);
	}
release_fnames:
	free(wstats->lk_fname);
release_fds:
	free(wstats->slock_fd);
out:
	return ret;
}

int create_shared_buffers(struct writer_stats *wstats, int count, char *fname)
{
	int i = 0;
	int buf_name_len = strlen(fname) + 1;
	int ret = -1;

	wstats->buf_fds = malloc(count * sizeof(int));
	if (!wstats->buf_fds) {
		printf("\nFailed to allocaet memory for buffers fds.\n");
		goto out;
	}

	wstats->buf_names = malloc(count * sizeof(char *));
	if (!wstats->buf_names) {
		printf("\nFailed to allocate memory for buffer name pointers.\n");
		goto release_buf_fds;
	}

	for (i = 0; i < count; i++)
	{
		wstats->buf_names[i] = malloc(buf_name_len);
		if (wstats->buf_names[i]) {
			printf("\nFailed to allocate memory for buffer name"
					" for buffer number %d.\n", i);
			goto release_buf_pointers; 
		}
	}

	for (i = 0; i < count; i++) {
		sprintf(wstats->buf_names[i], "%s%d", fname, i);
	}

	for (i = 0; i < count; i++) {
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

	for (i = 0; i < count; i++) {
		if (ftruncate(wstats->buf_fds[i], BUF_SIZE)) {
			printf("\nFailed to set size for shared buffer"
					" number %d\n", i);
			goto close_fds;
		}
	}
	ret = 0;

close_fds:
	for (i = 0; i < count; i++) {
		if (shm_unlink(wstats->buf_names[i])) {
			printf("\nFailed to destroy shared buffer"
					" number %d\n", i);
		}
	}
release_bufs:
	for (i = 0; i < count; i++) {
		free(wstats->buf_names[i]);
	}
release_buf_pointers:
	free(wstats->buf_names);
release_buf_fds:
	free(wstats->buf_fds);
out:
	return ret;
}
