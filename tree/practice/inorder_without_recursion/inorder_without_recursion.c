#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <stack.h>
#include <circular_queue.h>

void
inorder_without_stack(struct tree *root)
{
	struct tree *ptr = NULL;
	if (root) {
		while (1) {
			while (root) {
				push(root);
				root = root->left;
			}
			ptr = pop();
			if (!ptr)
				break;
			printf("%d\t", ptr->data);
			root = ptr->right;
		}
	}
}

void
lorder(struct tree *root)
{
	int i = 0;
	int child_count = 1;
	struct tree* ptr = NULL;

	addq(root);
	while (child_count) {
		i = child_count;
		child_count = 0;
		while (i) {
			ptr = delq();
			printf("%d\t", ptr->data);
			i--;

			if (ptr->left) {
				addq(ptr->left);
				child_count++;
			}
			if(ptr->right) {
				addq(ptr->right);
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

	root->left = NULL;
	root->right = NULL;
	root->data = 1;

	insert(root, 2);
	insert(root, 3);
	insert(root->left, 4);
	insert(root->left, 5);
	insert(root->right, 6);
	insert(root->right, 7);

	printf("\n\n");
	lorder(root);
	printf("\n");
	inorder_without_stack(root);
	printf("\n");

	return 0;
}
