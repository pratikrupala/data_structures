#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>

int search_index(int *in, int d, int start, int end)
{
	int i = 0;

	for (i = start; i <= end; i++) {
		if (in[i] == d)
			return i;
	}

	return - 1;
}

struct tree *generate_tree(int *in, int *pre, int start, int end, int *pre_index)
{
	struct tree *node = NULL;
	int index = -1;
	int p_index = *pre_index;

	if (start <= end) {
		*pre_index += 1;
		index = search_index(in, pre[p_index], start, end);
		node = malloc(sizeof(struct tree));
		if (!node) {
			printf("\nFailed to allocate memory for %d.\n", pre[start]);
			return NULL;
		}
		node->data = pre[p_index];
		node->left = generate_tree(in, pre, start, index - 1, pre_index);
		node->right = generate_tree(in, pre, index + 1, end, pre_index);
	}

	return node;
}

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

		if(!i)
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
	int i = 0;
	int n = -1;
	int *in = NULL;
	int *pre = NULL;
	int pre_index = 0;
	int ret = -1;
	struct tree *root = NULL;

	printf("Enter the size of the tree: ");
	scanf("%d", &n);
	if (!n) {
		printf("\nInvalid size of the array.\n");
		return -1;
	}

	in = malloc(n * sizeof(int));
	if (!in) {
		printf("Failed to allocate memory for inorder sequence.\n");
		goto out;
	}

	pre = malloc(n * sizeof(int));
	if (!pre) {
		printf("\nFailed to allocate memory for preorder sequence.\n");
		goto out1;
	}

	printf("\nEnter inorder sequence:\n");
	for (i = 0; i < n; i++) {
		printf("In[%d]: ", i);
		scanf("%d", &in[i]);
	}

	printf("\nEnter pre-order sequence:\n");
	for (i = 0; i < n; i++) {
		printf("Pre[%d]: ", i);
		scanf("%d", &pre[i]);
	}

	printf("\nInorder sequence:\n");
	for (i = 0; i < n; i++) {
		printf("%d\t", in[i]);
	}
	
	printf("\nPreorder sequence:\n");
	for (i = 0; i < n; i++) {
		printf("%d\t", pre[i]);
	}
	printf("\n");

	root = generate_tree(in, pre, 0, n - 1, &pre_index);
	if (!root) {
		printf("\nFailed to genrate tree.\n");
		goto out2;
	}

	printf("\nGenerated tree:\n");
	level_order(root);
	printf("\n");

out2:
	free(pre);
out1:
	free(in);
out:
	return ret;
}
