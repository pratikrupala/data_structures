#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>

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

void print_left_boundary(struct tree *root)
{
	if (!root)
		return;

	while (root->left) {
		printf("%d\t", root->data);
		root = root->left;
	}

	print_left_boundary(root->right);
}

void print_leaf_nodes(struct tree *root)
{
	if (!root)
		return;

	if (!(root->left) && !(root->right))
		printf("%d\t", root->data);

	print_leaf_nodes(root->left);
	print_leaf_nodes(root->right);
}

void print_right_boundary(struct tree *root)
{
	if (!root->right) {
		if (root->left)
			print_right_boundary(root->left);
		else
			return;
	}

	if (root->right)
		print_right_boundary(root->right);

	printf("%d\t", root->data);
}

void print_boundary(struct tree *root)
{
	if (root) {
		printf("%d\t", root->data);
		print_left_boundary(root->left);
		print_leaf_nodes(root);
//		print_right_boundary(root->right);
		printf("\n");
	}
}

int main(void)
{
	struct tree *root = malloc(sizeof(struct tree));
	if (!root) {
		printf("Failed to allocate memory for root.\n");
		return -1;
	}
	root->data = 1;
	root->left = root->right = NULL;

	insert(root, 2);
	insert(root, 3);
	insert(root->left, 4);
	insert(root->left, 5);
	insert(root->right, 6);
	insert(root->left->left, 7);
	insert(root->left->left, 8);
//	insert(root->right->left, 9);

	printf("\nOriginal Tree:\n");
	level_order(root);
	printf("\n");

	print_boundary(root);

	return 0;
}
