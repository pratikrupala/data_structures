#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include "write_helper.h"

struct writer_stats *wstats = NULL;

int main(int argc, char *argv[])
{
	int ret = -1;
	int option = 0;
	int buf_count = 0;
	char *file_name = NULL;
	char *lk_names = LOCK_NAMES;
	char *buf_names = BUF_NAMES;

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

	wstats = malloc(sizeof(struct writer_stats));
	if (!wstats) {
		printf("\nFailed to allocate memory for stats structure.\n");
		goto out;
	}

	ret = create_super_block(wstats, buf_count, file_name);
	if (ret) {
		printf("\nFailed to create super block.\n");
		goto release_wstats;
	}

	ret = create_lock_resources(wstats, buf_count, lk_names);
	if (ret) {
		printf("\nFailed to create lock resources.\n");
		goto release_super_block;
	}

	ret = create_shared_buffers(wstats, buf_count, buf_names);
	if (ret) {
		printf("\nFailed to allocate shared buffers.\n");
		goto release_lock_resources;
	}
	ret = 0;

release_lock_resources:
	// TBD
release_super_block:
	//TBD
release_wstats:
	free(wstats);
out:
	return ret;
}
