#define MAX_LEN	30

struct tree {
	struct tree *left;
	struct tree *right;
	int data;
};

int
insert(struct tree *p, int data)
{
	if (!p) {
		printf("Parent is NULL.\n");
		return -1;
	}

	if (!(p->left)) {
		p->left = (struct tree *) malloc(sizeof(struct tree));
		if (!(p->left)) {
			printf("Failed to allocate memory for data"
					" %d\n", data);
			return -1;
		}
		p->left->left = NULL;
		p->left->right = NULL;
		p->left->data = data;
	} else if (!(p->right)) {
		p->right = (struct tree *) malloc(sizeof(struct tree));
		if (!(p->right)) {
			printf("Failed to allocate memory for data"
					" %d\n", data);
			return -1;
		}
		p->right->left = NULL;
		p->right->right = NULL;
		p->right->data = data;
	} else {
		printf("\nInvalid condition. Both the child of the parent %d"
				" is already present\n", p->data);
		return -1;
	}

	return 0;
}
