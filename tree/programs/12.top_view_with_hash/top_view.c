#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>

#define HASH_SIZE 10

struct hnode {
	int data;
	struct hnode *next;
};

struct hnode *htable[HASH_SIZE];

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

void initialize_map(void)
{
	for (int i = 0; i < HASH_SIZE; i++) {
		htable[i] = NULL;
	}
}

void generate_map(struct tree *root, int degree)
{
	if (!root)
		return;

	struct hnode *node = malloc(sizeof(struct hnode));
	if (!node) {
		printf("\nFailed to allocate memory for %d, aborting.\n", root->data);
		return;
	}

	node->data = root->data;
	node->next = NULL;
	if (!htable[degree])
		htable[degree] = node;
	else {
		node->next = htable[degree];
		htable[degree] = node;
	}

	generate_map(root->left, degree - 1);
	generate_map(root->right, degree + 1);

}

void check_limits(struct tree *root, int *min, int *max, int degree)
{
	if (!root)
		return;

	if (degree < *min)
		*min = degree;
	else if (degree > *max)
		*max = degree;

	check_limits(root->left, min, max, degree - 1);
	check_limits(root->right, min, max, degree + 1);
}

void print_map()
{
	struct hnode *node = NULL;
	for (int i = 0; i < HASH_SIZE; i++) {
		node = htable[i];
		if (node) {
			while (node) {
				printf("%d\t", node->data);
				node = node->next;
			}
			printf("\n");
		} else {
			break;
		}
	}
}

void top_view(struct tree *root)
{
	int min = 0;
	int max = 0;
	int degree = 0;

	initialize_map();
	check_limits(root, &min, &max, degree);
	degree = -min;
	generate_map(root, degree);
	print_map();
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

	printf("Original tree:\n");
	level_order(root);
	printf("\n");

	printf("Top view:\n");
	top_view(root);
	printf("\n");

	return 0;
}
