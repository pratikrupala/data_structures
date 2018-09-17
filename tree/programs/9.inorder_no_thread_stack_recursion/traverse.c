#include <stdio.h>
#include <stdlib.h>
#include <tree_parent_pointer.h>
#include <circular_queue.h>

void non_threaded_inorder(struct tree *root)
{
	int down_done = 0;

	while (root) {
		if (!down_done) {
			while (root->left)
				root = root->left;
		}
		printf("%d\t", root->data);
		down_done = 1;

		if (root->right) {
			down_done = 0;
			root = root->right;
		} else if (root->parent) {
			while (root->parent && root == root->parent->right)
				root = root->parent;
			if (!root->parent)
				break;
			root = root->parent;
		} else {
			break;
		}
	}
}

void inorder(struct tree *root)
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
			if (root->parent)
				printf("%d(%d)\t", root->data, root->parent->data);
			else
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

int main(void)
{
	struct tree *root = malloc(sizeof(struct tree));
	if (!root) {
		printf("Failed to allocate memory for root node.\n");
		return -1;
	}
	root->data = 1;
	root->left = root->right = root->parent = NULL;

	insert(root, 2);
	insert(root, 3);
	insert(root->left, 4);
	insert(root->left, 5);
	insert(root->right, 6);
	insert(root->right, 7);

	printf("\nYour tree:\n");
	inorder(root);

	printf("\nInorder:\n");
	non_threaded_inorder(root);
	printf("\n");

	return 0;
}
