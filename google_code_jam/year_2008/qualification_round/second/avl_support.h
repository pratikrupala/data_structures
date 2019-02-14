#include "common.h"

struct avl_node {
	struct train_start_time *start_time;
	struct train_end_time *end_time;
	int base_factor;
	int freq;
	int color;
	struct avl_node *left;
	struct avl_node *right;
};

int tt_insert_node(struct avl_node **, struct train_start_time *,
		struct train_end_time *, int, int *);
void tt_avl_lvr(struct avl_node *);
int get_count(struct avl_node *, struct avl_node *, int *, int);
