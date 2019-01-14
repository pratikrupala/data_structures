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

#ifdef DEBUG
	printf("\n%s: Allocating memory for rwstats structure.\n", __func__);
#endif
	rwstats = malloc(sizeof(struct reader_writer_stats));
	if (!rwstats) {
		printf("\n%s: Failed to allocate memory for stats "
				"structure.\n", __func__);
		goto out;
	}
	rwstats->buf_count = buf_count;
	rwstats->buf_name_len = strlen(BUF_NAMES) + 1;
	rwstats->buf_name = malloc(rwstats->buf_name_len * sizeof(char));
	if (!rwstats->buf_name) {
		printf("\n%s: Failed to allocate memory for buffer names.\n",
				__func__);
		goto release_rwstats;
	}
	snprintf(rwstats->buf_name, rwstats->buf_name_len, "%s", BUF_NAMES);
	rwstats->buf_name[rwstats->buf_name_len] = '\0';

	printf("\nWriter: Creating a new super block.\n");
	ret = create_super_block(rwstats);
	if (ret) {
		printf("\n%s: Failed to create super block.\n", __func__);
		goto release_buf_name;
	}

	printf("\nWriter: Initializing the super block.\n");
	ret = initialize_super_block(rwstats);
	if (ret) {
		printf("\n%s: Failed to initialize super block.\n", __func__);
		goto release_super_block;
	}

	printf("\nWriter: Creating the shared buffers.\n");
	ret = create_shared_buffers_mappings(rwstats);
	if (ret) {
		printf("\n%s: Failed to allocate shared buffers.\n", __func__);
		goto release_super_block;
	}

	printf("\nWriter: Starting to filling up the shared buffers.\n");
	ret = fill_shared_buffers(rwstats, file_name);
	if (ret) {
		printf("\n%sFailed to fill up shared buffers.\n", __func__);
		goto release_buffer_resources;
	}
	ret = 0;

release_buffer_resources:
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
