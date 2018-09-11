#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>
#include <stack.h>

void
level_order(struct tree *root)
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

void
iterative_postorder(struct tree *root)
{
	struct tree *temp = NULL;
	if (!root)
		return;

	while (1) {
		if (!root) {
			root = pop();
			if (!root)
				break;
			if (root->right) {
				if (top >= 0 && (root->right->data == stk[top]->data)) {
					temp = pop();
					push(root);
					root = temp;
				} else {
					printf("%d\t", root->data);
					root = NULL;
				}
			} else {
				printf("%d\t", root->data);
				root = NULL;
			}
		} else {
			if (root->right)
				push(root->right);
			push(root);
			root = root->left;
		}
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
	root->right = root->left = NULL;

	insert(root, 2);
	insert(root, 3);
	insert(root->left, 4);
	insert(root->left, 5);
	insert(root->right, 6);
	insert(root->right, 7);

	printf("\nYou have entered: \n");
	level_order(root);

	printf("\nIterative post-order:\n");
	iterative_postorder(root);
	printf("\n");

	return 0;
}
