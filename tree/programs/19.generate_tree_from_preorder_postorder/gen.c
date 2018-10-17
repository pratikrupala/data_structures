#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>

int devide_postseq(int num, int *post, int s, int e)
{
	int i = 0;
	int j = 0;

	for (i = s; i <= e; i++, j++) {
		if (num == post[i])
			return j;
	}

	return -1;
}

struct tree *generate_tree(int *pre, int *post, int s, int e, int l)
{
	int index = 0;
	int delta = 0;
	struct tree *node = NULL;

	if (s > e)
		return NULL;

	node = malloc(sizeof(struct tree));
	if (!node) {
		printf("\nFailed to allocate memory for the node.\n");
		return NULL;
	}
	node->data = pre[0];
	node->left = node->right = NULL;

	if ((e - s) < 2)
		return node;

	index = devide_postseq(pre[1], post, s, e);
	if (index < 0) {
		printf("\nSomething went wrong.\n");
		return NULL;
	}

	delta = (index == 0)?0:index - 1;
	node->left = generate_tree(pre + 1, post, s, s + index, index + 1);
//	node->right = generate_tree(pre + index + 2, post, index + 1, l - 1, (l - index - 1));
	node->right = generate_tree(pre + index + 2, post, index + 1, l - 2, (l - index - 1));

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
	int *pre = NULL;
	int *post = NULL;
	int len = -1;
	int i = 0;
	struct tree *root = NULL;

	printf("\nEnter the length of the sequences: ");
	scanf("%d", &len);
	if (len < 1) {
		printf("\nInvalid length of the sequences.\n");
		return -1;
	}

	pre = malloc(len * sizeof(int));
	if (!pre) {
		printf("\nFailed to allocate memory for preorder sequence.\n");
		return -1;
	}

	printf("\nEnter pre-order sequence:\n");
	for (i = 0; i < len; i++) {
		printf("%d: ", i);
		scanf("%d", &pre[i]);
	}

	post = malloc(len * sizeof(int));
        if (!post) {
                printf("\nFailed to allocate memory for post-order sequence.\n");
                return -1;
        }

        printf("\nEnter post-order sequence:\n");
        for (i = 0; i < len; i++) {
                printf("%d: ", i);
                scanf("%d", &post[i]);
        }

	printf("\nYour entered pre-order sequence:\n");
	for (i = 0; i < len; i++) {
		printf("%d\t", pre[i]);
	}

	printf("\nYour entered post-order sequence:\n");
	for (i = 0; i < len; i++) {
		printf("%d\t", post[i]);
	}

	printf("\n");
	root = generate_tree(pre, post, 0, len - 1, len);
	if (!root) {
		printf("\nFailed to generate the tree.\n");
		return -1;
	}

	printf("\nFinal tree:\n");
	level_order(root);
	printf("\n");

	return 0;
}
