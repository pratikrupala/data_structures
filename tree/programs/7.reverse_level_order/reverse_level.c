#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>
#include <stack.h>

void level_order(struct tree *root)
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

void reverse_level_order(struct tree *root)
{
	int i = 0;
	int child_count = 1;

	reset_queue();
	addq(root);

	while (root) {
		i = child_count;
		child_count = 0;

		if (!i)
			break;

		while (i) {
			root = delq();
			push(root);
			i--;

			if (root->right) {
				addq(root->right);
				child_count++;
			}

			if (root->left) {
				addq(root->left);
				child_count++;
			}
		}
	}

	while ((root = pop())) {
		printf("%d\t", root->data);
	}

}

int main(void)
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

	printf("\nYour input:\n");
	level_order(root);

	printf("\nReverse level order:\n");
	reverse_level_order(root);
	printf("\n");

	return 0;
}
