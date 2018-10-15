#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>

int find_index(int *in, int num, int s, int e)
{
	int i = 0;

	for (i = s; i <= e;  i++) {
		if (num == in[i])
			return i;
	}
	return -1;
}

int my_search(int num, int *in, int s, int e)
{
	int i = 0;

	for (i = s; i <= e; i++) {
		if (num == in[i]) {
			return 0;
		}
	}
	return -1;
}

int *devide_level(int *in, int *level, int s, int e, int len)
{
	int i = 0;
	int j = 0;
	int *new_level = NULL;

	new_level = malloc((e - s + 1) * sizeof(int));
	if (!new_level) {
		printf("\nFailed to allocate memory for new level.\n");
		return NULL;
	}
	for (i = 0; i < len; i++) {
//		if (my_search(level[i], in, 0, (e - s)) == 0) {
		if (my_search(level[i], in, s, e) == 0) {
			new_level[j] = level[i];
			j++;
		}
	}
	return new_level;
}

struct tree *generate_tree(int *in, int *level, int start, int end, int len)
{
	struct tree *root = NULL;
	int *left_subtree = NULL;
	int *right_subtree = NULL;
	int index = -1;

	if (start > end)
		return NULL;

	root = malloc(sizeof(struct tree));
	if (!root) {
		printf("\nFailed to allocate memory for the node.\n");
		return NULL;
	}
	root->data = level[0];
	root->left = root->right = NULL;

	if (start == end)
		return root;

	index = find_index(in, level[0], start, end);
	if (index == -1) {
		printf("\nSomething went wrong.\n");
		return NULL;
	}

	left_subtree = devide_level(in, level, start, index - 1, len);
	right_subtree = devide_level(in, level, index + 1, end, len);

	root->left = generate_tree(in, left_subtree, start, index - 1, (index - start));
	root->right = generate_tree(in, right_subtree, index + 1, end, (end - index));

	free(left_subtree);
	free(right_subtree);
	return root;
}

void level_order(struct tree *root)
{
	int i = 0;
	int child_count = 1;

	if (!root)
		return;

	addq(root);

	while(root) {
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

int main(void)
{
	int *in = NULL;
	int *level = NULL;
	int len = -1;
	int i = 0;
	struct tree *root = NULL;

	printf("\nEnter the length of the inorder/level order: ");
	scanf("%d", &len);
	if (len < 1) {
		printf("\nInvalid length.\n");
		return -1;
	}

	in = malloc(len * sizeof(int));
	if (!in) {
		printf("\nFailed to allocate memory for inorder sequence.\n");
		return -1;
	}

	level = malloc(len * sizeof(int));
	if (!level) {
		printf("\nFailed to allocate memory for level order"
				" sequence.\n");
		return -1;
	}

	printf("\nEnter the inorder sequence:\n");
	for (i = 0; i < len; i++) {
		printf("in[%d]: ", i);
		scanf("%d", &in[i]);
	}

	printf("\nEnter the level order sequence:\n");
	for (i = 0; i < len; i++) {
		printf("level[%d]: ", i);
		scanf("%d", &level[i]);
	}

	printf("\nYour inorder sequence:\n");
	for (i = 0; i < len; i++) {
		printf("%d\t", in[i]);
	}

	printf("\nYour level order sequence:\n");
	for (i = 0; i < len; i++) {
		printf("%d\t", level[i]);
	}

	
	root = generate_tree(in, level, 0, len - 1, len);
	if (!root) {
		printf("\nNo tree returned.\n");
	}

	printf("\nFinal tree:\n");
	level_order(root);
	
	return 0;
}
