#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>

#define MAX_SIZE 10

struct tree *q1[MAX_SIZE];
struct tree *q2[MAX_SIZE];
int front1 = -1;
int rear1 = -1;
int front2 = -1;
int rear2 = -1;

int
is_full(struct tree **q)
{
	if (q == q1) {
		if (((front1 == -1) && (rear1 == MAX_SIZE - 1)) ||
			       (((rear1 + 1) % MAX_SIZE) == front1)) {
			return -1;
		}
	} else if (q == q2) {
		if (((front2 == -1) && (rear2 == MAX_SIZE - 1)) ||
			       (((rear2 + 1) % MAX_SIZE) == front2)) {
			return -1;
		}
	} else {
		return -1;
	}
	return 0;
}

int
enq(struct tree **q, struct tree *node)
{
	if (is_full(q)) {
		printf("\nQueue is full. Try later\n");
		return -1;
	}
	if (q == q1) {
		rear1 = (rear1 + 1) % MAX_SIZE;
		q[rear1] = node;
	} else {
		rear2 = (rear2 + 1) % MAX_SIZE;
		q[rear2] = node;
	}

	return 0;
}

int
is_empty(struct tree **q)
{
	int *rear = NULL;
	int *front = NULL;

	if (q == q1) {
		rear = &rear1;
		front = &front1;
	} else {
		rear = &rear2;
		front = &front2;
	}

	if ((*rear == -1) || (*rear == *front))
		return -1;

	return 0;
}

struct tree *
deq(struct tree **q)
{
	int *front = NULL;
	if (is_empty(q))
		return NULL;

	if (q == q1)
		front = &front1;
	else
		front = &front2;

	return q[++(*front)];
}

void
level_order(struct tree *root)
{
	int ret = -1;

	ret = enq(q1, root);
	if (ret) {
	}

	while (!is_empty(q1) || !is_empty(q2)) {
		while (!is_empty(q1)) {
			root = deq(q1);
			printf("%d\t", root->data);
			if (root->left) {
				enq(q2, root->left);
			}
			if (root->right) {
				enq(q2, root->right);
			}
		}
		printf("\n");
		while (!is_empty(q2)) {
			root = deq(q2);
			if (root->left) {
				enq(q1, root->left);
			}
			if (root->right) {
				enq(q1, root->right);
			}
			printf("%d\t", root->data);
		}
		printf("\n");
	}

}

void
print_tree(struct tree *root)
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

		while(i) {
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

int
main(void)
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

	print_tree(root);

	printf("\nLevel order:\n");
	level_order(root);

	return 0;
}
