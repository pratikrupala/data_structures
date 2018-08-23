#include <stdio.h>
#include <stdlib.h>
#include <circular_queue.h>

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

void
lorder(struct tree *root)
{
	struct tree *ptr = NULL;
	addq(root);

	while (1) {
		ptr = delq();
		if (!ptr) {
			break;
		}

		printf("\t %d", ptr->data);
		if (ptr->left) {
			addq(ptr->left);
		}
		if (ptr->right) {
			addq(ptr->right);
		}
	}
	printf("\n");
}

int
main(void)
{
	struct tree *root = (struct tree *) malloc(sizeof(struct tree));

	root->left = NULL;
	root->right = NULL;
	root->data = 1;

	insert(root, 2);
	insert(root, 3);
	insert(root->left, 4);
	insert(root->left, 5);
	insert(root->left->left, 6);
	insert(root->left->left, 7);

	lorder(root);

	return 0;
}
