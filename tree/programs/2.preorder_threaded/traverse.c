#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>

void
preorder_threaded(struct tree *root)
{
	struct tree *ptr = NULL;

	while (root) {
		if (!root->left) {
			printf("%d\t", root->data);
			root = root->right;
		} else {
			ptr = root->left;

			while (ptr->right != NULL && ptr->right != root) {
				ptr = ptr->right;
			}

			if (ptr->right == NULL) {
				printf("%d\t", root->data);
				ptr->right = root;
				root = root->left;
			} else {
				ptr->right = NULL;
				root = root->right;
			}
		}
	}
}

void
level_order(struct tree *root)
{
	int i = 0;
	int child_count = 1;

	addq(root);
	if (!root) {
		return;
	}

	while (root) {
		i = child_count;
		child_count = 0;
		
		if (!i) {
			break;
		}

		while (i) {
			root = delq();
			printf("%d\t", root->data);
			i--;

			if (root->left) {
				addq(root->left);
				child_count++;
			}
			if (root->right) {
				addq(root->right);
				child_count++;
			}
		}
		printf("\n");
	}
}

int
main(void)
{
	struct tree *root = malloc(sizeof(struct tree));
	if (!root) {
		printf("Failed to allocate memory for root node.\n");
		return -1;
	}
	root->data = 1;
	root->left = root->right = NULL;

	insert(root, 2);
	insert(root, 3);
	insert(root->left, 4);
	insert(root->left, 5);
	insert(root->right, 6);
	insert(root->right, 7);

	level_order(root);

	printf("Result: \n");
	preorder_threaded(root);
	printf("\n");

	return 0;
}
