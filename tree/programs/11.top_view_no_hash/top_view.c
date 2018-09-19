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

void find_limits(struct tree *root, int *max, int *min, int level)
{
	if (!root)
		return;

	if (level < *min)
		*min = level;
	else if (level > *max)
		*max = level;

	find_limits(root->left, max, min, level - 1);
	find_limits(root->left, max, min, level + 1);
}

void print_verticals(struct tree *root, int line, int level)
{
	if (!root)
		return;

	if (level == line) 
		printf("%d\t", root->data);

	print_verticals(root->left, line, level - 1);
	print_verticals(root->right, line, level + 1);
}

void top_view(struct tree *root)
{
	int max = 0;
	int min = 0;
	int degree = 0;

	if (!root)
		return;

	find_limits(root, &max, &min, degree);

	degree = 0;
	for (int i = min; i <= max; i++) {
		print_verticals(root, i, degree);
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
	root->left = root->right = NULL;

	insert(root, 2);
	insert(root, 3);
	insert(root->left, 4);
	insert(root->left, 5);
	insert(root->right, 6);
	insert(root->right, 7);

	printf("\nOriginal tree: \n");
	level_order(root);
	printf("\n");

	printf("\nTop view:\n");
	top_view(root);

	return 0;
}
