#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>
#include <stack.h>

#define STK_SIZE 10

int
is_full(int top)
{
	if (top == STK_SIZE - 1)
		return 1;

	return 0;
}

int
is_empty(int top)
{
	if (top == -1)
		return 1;

	return 0;
}

void
push_local(struct tree **stk, struct tree *node, int *top)
{
	if (is_full(*top))
		return;
	stk[++(*top)] = node;
}

struct tree *
pop_local(struct tree **stk, int *top)
{
	struct tree *node = NULL;
	if (is_empty(*top))
		return NULL;

	node = stk[(*top)--];
	return node;
}

void
iterative_postorder(struct tree *root)
{
	struct tree *stk1[10];
	struct tree *stk2[10];
	int s1top = -1;
	int s2top = -1;

	if (!root)
		return;
	
	push_local(stk1, root, &s1top);

	while (!is_empty(s1top)) {
		root = pop_local(stk1, &s1top);
		push_local(stk2, root, &s2top);

		if (root->left) {
			push_local(stk1, root->left, &s1top);
		}
		if (root->right) {
			push_local(stk1, root->right, &s1top);
		}
	}

	printf("\nPostorder traversal:\n");
	while (!is_empty(s2top)) {
		root = pop_local(stk2, &s2top);
		printf("%d\t", root->data);
	}

}

void
level_order(struct tree *root)
{
	int i = 0;
	int child_count = 1;
	if (!root)
		return;

	printf("\n");
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

int
main(void)
{
	struct tree *root = malloc(sizeof(struct tree));
	if (!root) {
		printf("Failed to allocate memory for the root node.\n");
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

	printf("\nYour entered tree:\n");
	level_order(root);

	iterative_postorder(root);
	printf("\n");

	return 0;
}
