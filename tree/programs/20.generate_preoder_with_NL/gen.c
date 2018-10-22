#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>

void level_order(struct tree *root)
{
	int i = 0;
	int child_count = 1;

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

struct tree *generate_tree(int *pre, char *nlpre, int l)
{
	struct tree *node = NULL;
	static int c = 0;

	node = malloc(sizeof(struct tree));
	if (!node) {
		printf("\nFailed to allocate memory for node"
				" %d\n", pre[0]);
		return NULL;
	}

	node->data = pre[c];
	node->left = node->right = NULL;
	c++;

	if (nlpre[c - 1] == 'L') {
		return node;
	}

	node->left = generate_tree(pre, nlpre, l);
	node->right = generate_tree(pre, nlpre, l);

	return node;
}

int main(void)
{
	int *pre = NULL;
	char *nlpre = NULL;
	int len = -1;
	int i = 0;
	struct tree *root = NULL;

	printf("\nEnter the length of the tree: ");
	scanf("%d", &len);
	if (len < 1) {
		printf("\nInvalid length.\n");
		return -1;
	}

	pre = malloc(len * sizeof(int));
	if (!pre) {
		printf("\nFailed to allocate memory.\n");
		return -1;
	}

	printf("\nEnter pre-order sequence:\n");
	for (i = 0; i < len; i++) {
		printf("%d: ", i);
		scanf("%d", &pre[i]);
	}

	printf("\nYour entered pre-order sequence:\n");
	for (i = 0; i < len; i++) {
		printf("%d\t", pre[i]);
	}

	nlpre = malloc(len * sizeof(char));
	if (!nlpre) {
		printf("\nFailed to allcoate memory for NL sequence.\n");
		return -1;
	}

	printf("\nEnter NL sequence for corresponding pre-order member: \n");
	for (i = 0; i < len; i++) {
		printf("%d: ", pre[i]);
		scanf(" %c", &nlpre[i]);
	}

	printf("\nYour entered NL sequence:\n");
	for (i = 0; i < len; i++) {
		printf("%c\t", nlpre[i]);
	}
	printf("\n");

	root = generate_tree(pre, nlpre, len);
	if (!root) {
		printf("\nFailed to generate tree.\n");
		return -1;
	}

	printf("\nFinal tree:\n");
	level_order(root);

	printf("\n\n");
	return 0;
}
