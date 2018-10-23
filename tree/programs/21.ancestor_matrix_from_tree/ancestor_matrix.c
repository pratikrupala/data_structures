#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>
#include <string.h>

int **ancestor_matrix = NULL;

int tree_size = 0;

struct lnode{
	struct tree *ptr;
	struct lnode *next;
	struct lnode *prev;
};

struct lnode *head;
struct lnode *tail;

void level_order(struct tree *root)
{
	int i = 0;
	int child_count = 1;

	if (!root) {
		return;
	}

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
				tree_size++;
			}

			if (root->right) {
				addq(root->right);
				child_count++;
				tree_size++;
			}
		}
		printf("\n");
	}
}

void list_add(struct lnode *node)
{
	if (head) {
		tail->next = node;
		node->prev = tail;
		tail = node;
	} else {
		head = node;
		tail = node;
	}
}

void list_remove(void)
{
	if (head == tail) {
		free(head);
		head = NULL;
		tail = NULL;
	} else {
		tail = tail->prev;
		free(tail->next);
		tail->next = NULL;
	}
}

void fill_matrix(int data)
{
	struct lnode *node = head;
	while (node) {
		ancestor_matrix[node->ptr->data][data] = 1;
		node = node->next;
	}
}

void generate_ancestor_matrix(struct tree *root)
{
	if (!root->left && !root->right) {
		goto end;
	}
	struct lnode *node = malloc(sizeof(struct lnode));
	node->ptr = root;
	node->next = NULL;
	node->prev = NULL;
	list_add(node);

	if (root->left)
		generate_ancestor_matrix(root->left);
	if (root->right)
		generate_ancestor_matrix(root->right);

	list_remove();
end:
	fill_matrix(root->data);
}

int main(void)
{
	int i = 0;
	int j = 0;
	struct tree *root = malloc(sizeof(struct tree));
	if (!root) {
		printf("\nFailed to allocate memory for root node.\n");
		return -1;
	}
/*
	root->data = 0;
	insert(root, 1);
	insert(root, 2);
	insert(root->left, 3);
	insert(root->left, 4);
	insert(root->right, 5);
	insert(root->right, 6);
*/
	root->data = 5;
	insert(root, 3);
	insert(root, 4);
	insert(root->left, 2);
	insert(root->left, 1);
	insert(root->right, 0);

	level_order(root);

	tree_size++;
	ancestor_matrix = malloc(tree_size * sizeof(int *));
	if (!tree_size) {
		printf("\nFailed to allocate memory for ancestor matrix.\n");
		return -1;
	}

	for (i = 0; i < tree_size; i++) {
		ancestor_matrix[i] = malloc(tree_size * sizeof(int));
		memset(ancestor_matrix[i], 0, tree_size);
	}

	printf("\nEmpty ancestor matrix:\n");
	for (i = 0; i < tree_size; i++) {
		for (j = 0; j < tree_size; j++) {
			printf("%d\t", ancestor_matrix[i][j]);
		}
		printf("\n");
	}

	generate_ancestor_matrix(root);

	printf("\nAncestor matrix:\n");
	for (i = 0; i < tree_size; i++) {
		for (j = 0; j < tree_size; j++) {
			printf("%d\t", ancestor_matrix[i][j]);
		}
		printf("\n");
	}

	return 0;
}
