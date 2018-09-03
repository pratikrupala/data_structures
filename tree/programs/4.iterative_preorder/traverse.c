#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <stack.h>
#include <circular_queue.h>

void
level_order(struct tree *root)
{
	int i = 0;
	int child_count = 1;

	if (!root)
		return;

	addq(root);
	while (root) {
		i = child_count;
		child_count = 0;

		if (!i)
			break;
		while (i)
		{
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

void
preorder_iterative(struct tree *root)
{
	if (!root)
		return;

	printf("\n");
	while (1) {
		while (root) {
			printf("%d\t", root->data);
			// TBD: if (root->right) is NULL
			// then we might skip pushing it.
			push(root);
			root = root->left;
		}
		while(!root) {
			root = pop();
			if (!root) {
				goto out;
			}
			root = root->right;
		}
	}
out:
	printf("\n");
	return;
}

int
main(void)
{
	struct tree *root = malloc(sizeof(struct tree));
	if (!root) {
		printf("\nNot able to allocate memory for root node.\n");
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

	preorder_iterative(root);

	return 0;
}
