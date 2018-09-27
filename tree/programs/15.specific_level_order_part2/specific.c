#include <stdio.h>
#include <stdlib.h>
#include <tree.h>

#define MAX_SIZE 20

struct qptr {
	struct tree *ptr;
	struct qptr *next;
};
struct qptr *qfront = NULL;
struct qptr *qrear = NULL;
struct qptr *s_top = NULL;

void push(struct tree *node)
{
	struct qptr *new_node = malloc(sizeof(struct qptr));
	new_node->ptr = node;
	new_node->next = NULL;
	if (!s_top) {
		s_top = new_node;
	} else {
		new_node->next = s_top;
		s_top = new_node;
	}
}

struct tree *pop()
{
	struct tree *node = NULL;
	struct qptr *ptr = NULL;
	if (s_top) {
		ptr = s_top;
		node = s_top->ptr;
		s_top = s_top->next;
	} else {
		return NULL;
	}
	free(ptr);

	return node;
}

void enq(struct tree *node)
{
	struct qptr *new_node = malloc(sizeof(struct qptr));
	new_node->ptr = node;
	new_node->next = NULL;
	if (!qrear) {
		qfront = qrear = new_node;
		qfront->next = qrear->next = new_node;
	} else if (qfront && (qfront == qfront->next)) {
		qfront->next = new_node;
		new_node->next = qrear->next;
		qrear->next = new_node;
		qrear = new_node;
	} else {
		new_node->next = qrear->next;
		qrear->next = new_node;
		qrear = new_node;
	}
}

struct tree *deq(void)
{
	struct tree *node = NULL;
	struct qptr *tmp = NULL;
	if (!qfront) {
		return NULL;
	} else {
		if (qrear != qfront) {
			qrear->next = qfront->next;
			tmp = qfront;
			node = qfront->ptr;
			qfront = qfront->next;
		} else {
			node = qfront->ptr;
			qfront = NULL;
			qrear = NULL;
		}
	}
	free(tmp);
	return node;
}

struct tree *my_node(int data)
{
	struct tree *node = malloc(sizeof(struct tree));
	if (!node) {
		printf("\nFailed to allocate memory for %d.\n", data);
		return NULL;
	}
	node->data = data;
	node->left = node->right = NULL;

	return node;
}

void level_order(struct tree *root)
{
	int i = 0;
	int child_count = 1;

	if (!root)
		return;

	enq(root);
	while (root) {
		i = child_count;
		child_count = 0;

		if (!i)
			break;

		while (i) {
			root = deq();
			printf("%d\t", root->data);
			i--;

			if (root->left) {
				enq(root->left);
				child_count++;
			}
			if (root->right) {
				enq(root->right);
				child_count++;
			}
		}
		printf("\n");
	}
}

void reinitialize_queue(void)
{
	qfront = NULL;
	qrear = NULL;
}

void specific_level_order(struct tree *root)
{
	if (!root)
		return;

//	printf("%d\t", root->data);
	push(root);

	if (root->left) {
		push(root->left);
		push(root->right);
	} else
		return;

	if (root->left->left) {
		enq(root->left);
		enq(root->right);
	}

	struct tree *first = NULL;
	struct tree *second = NULL;

	while (qfront) {
		first = deq();
		second = deq();

		push(first->left);
		push(second->right);
		push(first->right);
		push(second->left);
//		printf("%d\t%d\t", first->left->data, second->right->data);
//		printf("%d\t%d\t", first->right->data, second->left->data);

		if (first->left->left) {
			enq(first->left);
			enq(second->right);
			enq(first->right);
			enq(second->left);
		}
	}

	while (s_top) {
		first = pop();
		printf("%d\t", first->data);
	}
	printf("\n");
}

int main(void)
{
	struct tree *root = my_node(1);

	root->left = my_node(2);
	root->right = my_node(3);
	root->left->left = my_node(4);
	root->left->right = my_node(5);
	root->right->left = my_node(6);
	root->right->right = my_node(7);
	root->left->left->left = my_node(8);
	root->left->left->right = my_node(9);
	root->left->right->left = my_node(10);
	root->left->right->right = my_node(11);
	root->right->left->left = my_node(12);
	root->right->left->right = my_node(13);
	root->right->right->left = my_node(14);
	root->right->right->right = my_node(15);

	printf("\nOriginal tree:\n");
	level_order(root);

	reinitialize_queue();
	printf("\nSpecific level order:\n");
	specific_level_order(root);

	return 0;
}
