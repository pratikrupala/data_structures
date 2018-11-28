struct super_block {
	int sblock_len;
	int buf_count;
	int buf_name_len;
	int buf_lk_name_len;
	char *buf_name;
	char *buf_lk_name;
	int *producer_map;
	int *consumer_map;
};
