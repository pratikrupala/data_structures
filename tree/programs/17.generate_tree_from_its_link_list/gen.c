#include <stdio.h>
#include <stdlib.h>
#include <tree.h>
#include <circular_queue.h>

struct lnode {
	int data;
	struct lnode * next;
};

struct lnode *head = NULL;
struct lnode *last = NULL;

int list_add(int data)
{
	int res = -1;
	struct lnode *new_node = malloc(sizeof(struct lnode));
	if (!new_node) {
		printf("\nFailed to allocate memory for %d.\n", data);
		goto out;
	}
	new_node->data = data;
	new_node->next = NULL;

	if (head) {
		last->next = new_node;
		last = last->next;
	} else {
		head = new_node;
		last = new_node;
	}

out:
	return res;
}

void iter_list(void) {
	struct lnode *iter = head;
	while (iter) {
		printf("%d\t", iter->data);
		iter = iter->next;
	}
}

int find_node_data(int index, int *val)
{
	struct lnode *iter = head;
	index--;
	while(index) {
		iter = iter->next;
		if (!iter)
			return -1;
		index--;
	}
	*val = iter->data;
	return 0;
}

struct tree *generate_tree(int index) {
	struct tree *node = NULL;
	int val = -1;
	int res = -1;

	node = malloc(sizeof(struct tree));
	res = find_node_data(index, &val);
	if (res)
		return NULL;
	node->data = val;

	node->left = generate_tree(index * 2);
	node->right = generate_tree((index * 2) + 1);

	return node;
}

void level_order(struct tree *root)
{
	int i = 0;;
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

int main(void)
{
	int i = 0;
	int len = 0;
	int val = -1;
	struct tree *root = NULL;

	printf("\nEnter the length of the linked list: ");
	scanf("%d", &len);
	if (len < 1) {
		printf("\nInvalid length of the linked list.\n");
		return -1;
	}

	printf("\nEnter the values of the linked list.\n");
	for (i = 0; i < len; i++) {
		printf("%d: ", i);
		scanf("%d", &val);
		list_add(val);
	}

	printf("\nOriginal list:\n");
	iter_list();
	printf("\n");
	
	root = generate_tree(1);
	if (!root) {
		printf("\nFailed to generate tree.\n");
		return -1;
	}

	printf("\n");
	level_order(root);

	return 0;
}
