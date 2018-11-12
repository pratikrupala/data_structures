#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 1024
#define LOCK_NAMES "slocks"
#define BUF_NAMES "sbufs"
#define SBLOCK_LK_NAME "sblock_lock"

struct writer_stats {
	int *sblock_fd;
	char *sblock_lk_name;
	int *slock_fd;	/* Shared lock fds */
	char **lk_fname; /* Shmem names for locks */
	int *buf_fds;	/* Data buffer fds */
        char **buf_names; /* Buffer names */
};

int create_lock_resources(struct writer_stats *, int, char *);
int create_shared_buffers(struct writer_stats *, int, char *);
int create_super_block(struct writer_stats *, int, char *);
