#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>

#define DSIZE 30

struct tree *q1[DSIZE];
struct tree *q2[DSIZE];

int f1 = -1;
int f2 = -1;
int r1 = -1;
int r2 = -1;

int is_full(struct tree **q)
{
	int *f = NULL;
	int *r = NULL;
	if (q == q1) {
		f = &f1;
		r = &r1;
	} else if (q == q2) {
		f = &f2;
		r = &r2;
	} else {
		printf("\nInvalid queue for enqueue.\n");
		return -1;
	}

	if ((*f == -1 && *r == DSIZE - 2) || (*r == *f - 1))
		return 1;

	return 0;
}

int enqueue(struct tree **q, struct tree *node)
{
	int *r = NULL;
	if (q == q1) {
		r = &r1;
	} else if (q == q2) {
		r = &r2;
	} else {
		printf("\nInvalid queue for enqueue.\n");
		return -1;
	}

	if (is_full(q)) {
		printf("\nQueue is full\n");
		return -1;
	}

	q[++(*r)] = node;
	return 0;
}

int is_empty(struct tree **q)
{
	int *f = NULL;
	int *r = NULL;
	if (q == q1) {
		f = &f1;
		r = &r1;
	} else if (q == q2) {
		f = &f2;
		r = &r2;
	} else {
		printf("\nInvalid queue for enqueue.\n");
		return -1;
	}

	if (*f == *r)
		return 1;

	return 0;
}

struct tree *dequeue(struct tree **q)
{
	int *f = NULL;
	if (q == q1) {
		f = &f1;
	} else if (q == q2) {
		f = &f2;
	} else {
		printf("\nInvalid queue for enqueue.\n");
		return NULL;
	}

	if (is_empty(q)) {
		printf("\nQueue is empty for dequeue.\n");
		return NULL;
	}

	return q[++(*f)];
}

void double_queue(struct tree *root)
{
	int res = -1;
	if (!root)
		return;

	res = enqueue(q1, root);
	if (res) {
		printf("\nSomething went wrong in enqueue.\n");
		return;
	}
	while (!is_empty(q1) || !is_empty(q2)) {
		while (!is_empty(q1)) {
			root = dequeue(q1);
			printf("%d\t", root->data);

			if (root->left) {
				res = enqueue(q2, root->left);
				if (res) {
					printf("\nSomething went wrong in"
							" enqueue.\n");
					return;
				}
			}
			if (root->right) {
				res = enqueue(q2, root->right);
				if (res) {
					printf("\nSomething went wrong in"
							" enqueue.\n");
					return;
				}
			}
		}
		printf("\n");
		while (!is_empty(q2)) {
			root = dequeue(q2);
			printf("%d\t", root->data);

			if (root->left) {
				res = enqueue(q1, root->left);
				if (res) {
					printf("\nSomething went wrong in"
							" enqueue.\n");
					return;
				}
			}
			if (root->right) {
				res = enqueue(q1, root->right);
				if (res) {
					printf("\nSomething went wrong in"
							" enqueue.\n");
					return;
				}
			}
		}
		printf("\n");
	}
}

void single_queue(struct tree *root)
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

void specific_level_order(struct tree *root)
{
	if (!root)
		return;

	printf("%d\n", root->data);

	if (root->left)
		printf("%d\t%d\n", root->left->data, root->right->data);
	else
		return;

	if (!root->left->left)
		return;

	addq(root->left);
	addq(root->right);

	struct tree *first = NULL;
	struct tree *second = NULL;
	while (!q_empty()) {
		first = delq();
		second = delq();

		printf("%d\t%d\t%d\t%d\n", first->left->data,
				second->right->data, first->right->data,
				second->left->data);

		if (first->left->left) {
			addq(first->left);
			addq(second->right);
			addq(first->right);
			addq(second->left);
		}
	}
}

int main(void)
{
	struct tree *root= malloc(sizeof(struct tree));
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
	insert(root->left->left, 8);
	insert(root->left->left, 9);
	insert(root->left->right, 10);
	insert(root->left->right, 11);
	insert(root->right->left, 12);
	insert(root->right->left, 13);
	insert(root->right->right, 14);
	insert(root->right->right, 15);

	printf("\nSingle queue:\n");
	single_queue(root);

	/*
	printf("\nDouble queue:\n");
	double_queue(root);
	*/

	printf("\nSpecific level order:\n");
	specific_level_order(root);

	return 0;
}
