#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>

void
lorder(struct tree *root)
{
	struct tree *ptr = NULL;
	int inner_count = 0;
	int child_count = 1;
	addq(root);

	while (1) {
		inner_count = child_count;
		child_count = 0;
		if (!inner_count)
			break;
		while (inner_count) {
			ptr = delq();

			printf("\t %d", ptr->data);
			if (ptr->left) {
				addq(ptr->left);
				child_count++;
			}
			if (ptr->right) {
				addq(ptr->right);
				child_count++;
			}
			inner_count--;
		}
		printf("\n");
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
//	insert(root->left->left, 6);
//	insert(root->left->left, 7);
	insert(root->right, 6);
	insert(root->right, 7);

	lorder(root);

	return 0;
}
