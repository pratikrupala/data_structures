#include "common.h"

#define	OUTPUT_FILE	"output.txt"

int map_super_block(struct reader_writer_stats *);
int consume_shared_buffers(struct reader_writer_stats *, char *);
int read_super_block(struct reader_writer_stats *);
