#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>

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
