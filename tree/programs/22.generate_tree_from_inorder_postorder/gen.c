#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>

int search_index(int num, int *arr, int start, int end)
{
	int i = 0;

	for (i = start; i <= end; i++) {
		if (num == arr[i]) {
			return i;
		}
	}

	return -1;
}

struct tree *generate_tree(int *in, int *post, int istart, int iend, int *pindex)
{
	if (istart > iend)
		return NULL;

	int in_index = -1;

	struct tree *node = malloc(sizeof(struct tree));
	if (!node) {
		printf("\nFailed to allocate memory for new node.\n");
		return NULL;
	}
	node->data = post[*pindex];
	node->left = node->right = NULL;
	(*pindex)--;

	if (istart == iend)
		goto out;

	in_index = search_index(node->data, in, istart, iend);
	if (in_index == -1) {
		printf("\nSomething went wrong for in_index.\n");
		return NULL;
	}

	node->right = generate_tree(in, post, (in_index + 1), iend,
			pindex);
	node->left = generate_tree(in, post, istart, (in_index - 1),
			pindex);
out:
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
	int i = 0;
	int *in = NULL;
	int *post = NULL;
	int len = -1;
	int pindex = -1;
	struct tree *root = NULL;

	printf("\nEnter the length of the tree: ");
	scanf("%d", &len);
	if (len < 1) {
		printf("\nInvalid length for the tree.\n");
		return -1;
	}

	in = malloc(len * sizeof(int));
	if (!in) {
		printf("\nFailed to allocate memory for in-order sequence.\n");
		return -1;
	}

	printf("\nEnter the in-order sequence:\n");
	for (i = 0; i < len; i++) {
		printf("%d: ", i);
		scanf("%d", &in[i]);
	}

	post = malloc(len * sizeof(int));
	if (!post) {
		printf("\nFailed to allocate memory for post-order sequence.\n");
		return -1;
	}

	printf("\nEnter the post-order sequence:\n");
	for (i = 0; i < len; i++) {
		printf("%d: ", i);
		scanf("%d", &post[i]);
	}

	printf("\nYour entered in-order sequence:\n");
	for (i = 0; i < len; i++) {
		printf("%d\t", in[i]);
	}

	printf("\nYour entered post-order sequence:\n");
	for (i = 0; i < len; i++) {
		printf("%d\t", post[i]);
	}

	pindex = len - 1;
	root = generate_tree(in, post, 0, len - 1, &pindex);
	if (!root) {
		printf("\nFailed to generate tree.\n");
		return -1;
	}

	printf("\nFinal tree:\n");
	level_order(root);

	printf("\n\n");
	return 0;
}
