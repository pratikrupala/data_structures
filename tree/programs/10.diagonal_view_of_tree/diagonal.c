#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>

#define HASH_SIZE 10

struct hash_node {
	int value;
	struct hash_node *next;
};

struct hash_node *htable[HASH_SIZE];

void initialize_htable(void)
{
	for (int i = 0; i < (HASH_SIZE - 1); i++) {
		htable[i] = NULL;
	}
}

void hashify(int data, int key)
{
	struct hash_node *node = malloc(sizeof(struct hash_node));

	node->value = data;
	node->next = NULL;

	if (!htable[key]) {
		htable[key] = node;
	} else {
		node->next = htable[key];
		htable[key] = node;
	}
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

void diagonal_print(struct tree *root, int level)
{
	if (!root)
		return;

	hashify(root->data, level);

	if (root->left)
		diagonal_print(root->left, level + 1);

	if (root->right)
		diagonal_print(root->right, level);
}

void print_hashes(void)
{
	struct hash_node *ptr = NULL;
	for (int i = 0; i < HASH_SIZE - 1; i++) {
		ptr = htable[i];
		if (ptr) {
			while (ptr) {
				printf("%d\t", ptr->value);
				ptr = ptr->next;
			}
			printf("\n");
		} else {
			break;
		}
	}
}

int main(void)
{
	int level = 0;
	struct tree *root = malloc(sizeof(struct tree));
	if (!root) {
		printf("\nFailed to allocate memory for root node.\n");
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

	level_order(root);
	printf("\n");

	initialize_htable();
	diagonal_print(root, level);
	printf("\nDiagonal_print: \n");
	print_hashes();
	printf("\n");

	return 0;
}
