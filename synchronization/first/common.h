#ifndef _COMMON_H
#define _COMMON_H

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
#define MAX_SIZE 1024
#define SBLOCK_NAME "sblock"
#define LOCK_NAMES "slocks"
#define BUF_NAMES "sbufs"
#define SBLOCK_LK_NAME "sblock_lock"

struct reader_writer_stats {
	int buf_count;
	char *buf_name;
	int buf_name_len;
	int sblock_fd;
	int super_block_size;
	int bitmap_offset;
	int sblock_lk_fd;
	char *sblock_lk_name;
	int *buf_fds;	/* Data buffer fds */
        char **buf_names; /* Buffer names */
};

struct super_block {
	int sblock_len;
	int buf_count;
	int buf_name_len;
	int buf_lk_name_len;
	char *buf_name;
	char *buf_lk_name;
	int *buffer_map;
};

void bitmap_set_bit(char *, int);
void bitmap_clear_bit(char *, int);
int bitmap_get_bit(char *, int);
int create_super_block(struct reader_writer_stats *);
int create_shared_buffers_mappings(struct reader_writer_stats *);
void **open_all_shared_bufs(struct reader_writer_stats *);

#endif
