struct super_block {
	char *buf_names;
	char *buf_lk_names;
	int buf_count;
	int *producer_map;
	int *consumer_map;
};
