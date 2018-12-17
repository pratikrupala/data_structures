#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include "write_helper.h"

struct reader_writer_stats *rwstats = NULL;

int main(int argc, char *argv[])
{
	int ret = -1;
	int option = 0;
	int buf_count = 0;
	char *file_name = NULL;

	if (argc != 5) {
		printf("Invalid usage.\n");
		printf("Usage: writer -n <num_of_bufs> -f <input_file_name>\n");
		goto out;
	}

	while ((option = getopt(argc, argv,"n:f:")) != -1) {
		switch (option) {
			case 'n': buf_count = atoi(optarg);
				   break;
			case 'f': file_name = malloc(strlen(optarg));
				   strcpy(file_name, optarg);
				   break;
			default: printf("Usage: writer -n <num_of_bufs>"
						 " -f <input_file_name>\n");
				 goto out;
		}
	}

	rwstats = malloc(sizeof(struct reader_writer_stats));
	if (!rwstats) {
		printf("\nFailed to allocate memory for stats structure.\n");
		goto out;
	}
	rwstats->buf_count = buf_count;
	rwstats->buf_name_len = strlen(BUF_NAMES) + 1;
	rwstats->buf_name = malloc(rwstats->buf_name_len * sizeof(char));
	if (!rwstats->buf_name) {
		printf("\nFailed to allocate memory for buffer names.\n");
		goto release_rwstats;
	}
	snprintf(rwstats->buf_name, rwstats->buf_name_len, "%s", BUF_NAMES);
	rwstats->buf_name[rwstats->buf_name_len] = '\0';

	ret = create_super_block(rwstats);
	if (ret) {
		printf("\nFailed to create super block.\n");
		goto release_buf_name;
	}

	ret = initialize_super_block(rwstats);
	if (ret) {
		printf("\nFailed to initialize super block.\n");
		goto release_super_block;
	}

	/*
	ret = create_lock_resources(rwstats);
	if (ret) {
		printf("\nFailed to create lock resources.\n");
		goto release_super_block;
	}
	*/

	ret = create_shared_buffers(rwstats);
	if (ret) {
		printf("\nFailed to allocate shared buffers.\n");
		goto release_lock_resources;
	}

	ret = fill_shared_buffers(rwstats, file_name);
	if (ret) {
		printf("\nFailed to fill up shared buffers.\n");
		goto release_buffer_resources;
	}
	ret = 0;

release_buffer_resources:
	//TBD
release_lock_resources:
	//TBD
release_super_block:
	//TBD
release_buf_name:
	//TBD
release_rwstats:
	free(rwstats);
out:
	return ret;
}
