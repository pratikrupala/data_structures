#include <stdio.h>
#include <stdlib.h>
#include "avl_support.h"

void recalculate_balance_factor(struct avl_node *node)
{
	int left_hight = 0;
	int right_hight = 0;

	if(!node)
		return;
	if ((node->left) && (!node->right)) {
		node->base_factor = 1;
	} else if ((!node->left) && (node->right)) {
		node->base_factor = -1;
	} else {
		if (node->left)
			left_hight = node->left->base_factor;
		if (node->right)
			right_hight = node->right->base_factor;
		if ((left_hight == 1 || left_hight == -1) &&
				(right_hight == 0))
			node->base_factor = 1;
		else if ((right_hight == 1 || right_hight == -1) &&
				(left_hight == 0))
			node->base_factor = -1;
		else
			node->base_factor = 0;
	}
}

void avl_left_rotate(struct avl_node **root)
{
	struct avl_node *child = (*root)->left;
	struct avl_node *grandchild = NULL;

	if (!child)
		return;
	if (child->base_factor == 1) {
		(*root)->left = child->right;
		child->right = (*root);
		(*root)->base_factor = 0;
		(*root) = child;
	} else {
		grandchild = child->right;
		if (!grandchild)
			return;
		(*root)->left = grandchild->right;
		grandchild->right = (*root);
		child->right = grandchild->left;
		grandchild->left = child;
		(*root) = grandchild;
	}
	recalculate_balance_factor((*root)->left);
	recalculate_balance_factor((*root)->right);
	recalculate_balance_factor((*root));
}

void avl_right_rotate(struct avl_node **root)
{
	struct avl_node *child = (*root)->right;
	struct avl_node *grandchild = NULL;
	if (!child)
		return;
	if (child->base_factor == -1) {
		(*root)->right = child->left;
		child->left = (*root);
		(*root)->base_factor = 0;
		(*root) = child;
	} else {
		grandchild = child->left;
		if (!grandchild)
			return;
		(*root)->right = grandchild->left;
		grandchild->left = (*root);
		child->left = grandchild->right;
		grandchild->right = child;
		(*root) = grandchild;
	}
	recalculate_balance_factor((*root)->left);
	recalculate_balance_factor((*root)->right);
	recalculate_balance_factor((*root));
}

int char_compare(struct avl_node *root, char c)
{
	char rc = root->c;

	if (rc < c)
		return 1;
	else if (rc > c)
		return -1;
	else
		return 0;
}

struct avl_node *find_lowest_val_node(struct avl_node *root)
{
	while (root->left)
		root = root->left;

	return root;
}

int delete_char(struct avl_node **root, char c, int *unbalanced)
{
	int ret = -1;

	if (!(*root)) {
		*unbalanced = 0;
		goto out;
	} else if (char_compare(*root, c) == -1) {
		if (delete_char(&(*root)->left, c, unbalanced)) {
			/*
			printf(\"Could not find the char to be deleted. \"
					\"Aborting.\n\");
			goto out;
			*/
		}
		recalculate_balance_factor((*root)->left);
		recalculate_balance_factor((*root)->right);
		recalculate_balance_factor((*root));
		if (*unbalanced) {
			switch((*root)->base_factor) {
				case -1:
					(*root)->base_factor = 0;
					break;
				case 0:
					(*root)->base_factor = 1;
					break;
				case 1:
					avl_left_rotate(root);
					break;
				default:
					printf("Invalid case for the left side"
						" of the tree in deletion. "
						"Aborting.\n");
					goto out;
		}
		}
	} else if (char_compare(*root, c) == 1) {
		if (delete_char(&(*root)->right, c, unbalanced)) {
			/*
			printf(\"Could not find the char to be deleted. \"
					\"Aborting.\n\");
			*/
			goto out;
		}
		recalculate_balance_factor((*root)->left);
		recalculate_balance_factor((*root)->right);
		recalculate_balance_factor((*root));
		if (*unbalanced) {
			switch ((*root)->base_factor) {
				case 1:
					(*root)->base_factor = 0;
					break;
				case 0:
					(*root)->base_factor = -1;
					break;
				case -1:
					avl_right_rotate(root);
					break;
				default:
					printf("Invalid case for right side of"
						" the AVL tree in deletion. "
						"Aborting.\n");
					goto out;
			}
		}
	} else {
		*unbalanced = 1;
		struct avl_node *tmp = NULL;
		if ((*root)->left == NULL && (*root)->right == NULL) {
			tmp = *root;
			*root = NULL;
			free(tmp);
		} else if ((*root)->left == NULL || (*root)->right == NULL) {
			tmp = (*root)->left ? (*root)->left : (*root)->right;
			**root = *tmp;
			free(tmp);
		} else {
			tmp = find_lowest_val_node((*root)->right);
			(*root)->c = tmp->c;
			if (delete_char(&(*root)->right, tmp->c,
				unbalanced)) {
				/*
				printf(\"Could not find the char to be deleted. \"
					\"Aborting.\n\");
				*/
				goto out;
			}
			*unbalanced = 1;
//			free(tmp);
		}
	}
	ret = 0;
out:
	return ret;
}

int insert_char(struct avl_node **root, char c, int *unbalanced)
{
	struct avl_node *node = NULL;
	int ret = -1;

	if (!(*root)) {
		node = malloc(sizeof(struct avl_node));
		if (!node) {
			printf("Failed to allocate memory for node.\n");
			goto out;
		}
		node->base_factor = 0;
		node->c = c;
		node->left = node->right = NULL;
		*root = node;
		*unbalanced = 1;
//		free(node);
	} else if (char_compare(*root, c) == -1 ||
			char_compare(*root, c) == 0) {
		if (insert_char(&(*root)->left, c, unbalanced)) {
			printf("Failed to insert the node in AVL tree. "
					"Aborting.\n");
			goto out;
		}
		if (*unbalanced) {
			switch((*root)->base_factor) {
				case -1:
					(*root)->base_factor = 0;
					break;
				case 0:
					(*root)->base_factor = 1;
					break;
				case 1:
					avl_left_rotate(root);
					break;
				default:
					printf("Invalid case for the left side"
							" of the tree. "
							"Aborting.\n");
					goto out;
		}
		}
	} else if (char_compare(*root, c) == 1) {
		if (insert_char(&(*root)->right, c, unbalanced)) {
			printf("Failed to insert the node in AVL tree. "
					"Aborting.\n");
			goto out;
		}
		if (*unbalanced) {
			switch ((*root)->base_factor) {
				case 1:
					(*root)->base_factor = 0;
					break;
				case 0:
					(*root)->base_factor = -1;
					break;
				case -1:
					avl_right_rotate(root);
					break;
				default:
					printf("Invalid case for right side of"
							" the AVL tree. "
							"Aborting.\n");
					goto out;
			}
		}
	}
	ret = 0;
out:
	return ret;
}

void avl_lvr(struct avl_node *root)
{
	if (root) {
		avl_lvr(root->left);
		printf("%c\t", root->c);
		avl_lvr(root->right);
	}
}

/*
int modified_compare(struct avl_node *node, int hour, int min)
{
	if (node->start_time->start_hour > hour)
		return 1;
	if (node->start_time->start_hour < hour)
		return -1;
	if (node->start_time->start_min > min)
		return 1;
	if (node->start_time->start_min < min)
		return -1;

	return 0;
}
*/

struct avl_node *modified_avl_search(struct avl_node *tree, char c)
{
	struct avl_node *node = NULL;	

	if (!tree)
		return NULL;

	if (tree->c > c) {
		node = modified_avl_search(tree->left, c);
	} else if (tree->c < c) {
		node = modified_avl_search(tree->right, c);
	} else {
		return tree;
	}

	return node;
}

/*
int get_count(struct avl_node *first, struct avl_node *second, int *count,
		int turn_around_time)
{
	struct avl_node *node = NULL;
	int hour = 0;
	int min = 0;
	if (second) {
		if (get_count(first, second->left, count, turn_around_time))
			return -1;
		hour = second->end_time->end_hour;
		min = second->end_time->end_min;
		min += turn_around_time;
		if (min >= 60) {
			min -= 60;
			hour++;
		}
again:
		node = modified_avl_search(first, hour, min);
		if (node)
			(*count)--;
		else
			return -1;
		if (!(*count))
			return -1;
		else if (second->freq > 1) {
			second->freq -= 1;
			goto again;
		}
		if (get_count(first, second->right, count, turn_around_time))
			return -1;
	}

	return 0;
}
*/
