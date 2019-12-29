struct avl_node {
	char c;
	int base_factor;
	struct avl_node *left;
	struct avl_node *right;
};

struct avl_node *modified_avl_search(struct avl_node *, char);
int insert_char(struct avl_node **, char, int *);
int delete_char(struct avl_node **, char, int *);
void avl_lvr(struct avl_node *);
int get_count(struct avl_node *, struct avl_node *, int *, int);
