#include "common.h"

int create_shared_buffers(struct reader_writer_stats *);
int initialize_super_block(struct reader_writer_stats *);
int fill_shared_buffers(struct reader_writer_stats *, char *);
