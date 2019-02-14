#include <stdio.h>
#include <stdlib.h>
#include "avl_support.h"

void recalculate_balance_factor(struct avl_node *node)
{
	int left_hight = 0;
	int right_hight = 0;

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

	if (child->base_factor == 1) {
		(*root)->left = child->right;
		child->right = (*root);
		(*root)->base_factor = 0;
		(*root) = child;
	} else {
		grandchild = child->right;
		(*root)->left = grandchild->right;
		grandchild->right = (*root);
		child->right = grandchild->left;
		grandchild->left = child;
		(*root) = grandchild;
	}
	recalculate_balance_factor((*root)->left);
	recalculate_balance_factor((*root)->right);
	recalculate_balance_factor((*root));
	/*
	left_hight = (*root)->left->base_factor;
	right_hight = (*root)->right->base_factor;
	if ((left_hight == 1 || left_hight == -1) &&
			(right_hight == 0))
		(*root)->base_factor = 1;
	else if ((right_hight == 1 || right_hight == -1) &&
			(left_hight == 0))
		(*root)->base_factor = -1;
	else
		(*root)->base_factor = 0;
	*/
}

void avl_right_rotate(struct avl_node **root)
{
	struct avl_node *child = (*root)->right;
	struct avl_node *grandchild = NULL;
	if (child->base_factor == -1) {
		(*root)->right = child->left;
		child->left = (*root);
		(*root)->base_factor = 0;
		(*root) = child;
	} else {
		grandchild = child->left;
		(*root)->right = grandchild->left;
		grandchild->left = (*root);
		child->left = grandchild->right;
		grandchild->right = child;
		(*root) = grandchild;
	}
	recalculate_balance_factor((*root)->left);
	recalculate_balance_factor((*root)->right);
	recalculate_balance_factor((*root));
	/*
	left_hight = (*root)->left->base_factor;
	right_hight = (*root)->right->base_factor;
	if ((left_hight == 1 || left_hight == -1) &&
			(right_hight == 0))
		(*root)->base_factor = 1;
	else if ((right_hight == 1 || right_hight == -1) &&
			(left_hight == 0))
		(*root)->base_factor = -1;
	else
		(*root)->base_factor = 0;
	*/
}

int tt_time_compare(struct avl_node *root, struct train_start_time *tst,
		struct train_end_time *tet, int is_start)
{
	struct train_start_time *root_tst = root->start_time;
	struct train_end_time *root_tet = root->end_time;
	int ret = 0;

	if (is_start) {
		if (root_tst->start_hour < tst->start_hour)
			return 1;

		if (root_tst->start_hour > tst->start_hour)
			return -1;

		if (root_tst->start_min < tst->start_min)
			return 1;

		if (root_tst->start_min > tst->start_min)
			return -1;
	} else {
		if (root_tet->end_hour < tet->end_hour)
			return 1;

		if (root_tet->end_hour > tet->end_hour)
			return -1;

		if (root_tet->end_min < tet->end_min)
			return 1;

		if (root_tet->end_min > tet->end_min)
			return -1;
	}
	return ret;
}

int tt_insert_node(struct avl_node **root, struct train_start_time *tst,
		struct train_end_time *tet, int is_start, int *unbalanced)
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
		node->freq = 1;
		node->color = 0;
		node->left = node->right = NULL;
		node->start_time = tst;
		node->end_time = tet;
		*root = node;
		*unbalanced = 1;
	} else if (tt_time_compare(*root, tst, tet, is_start) == -1) {
		if (tt_insert_node(&(*root)->left, tst, tet, is_start,
					unbalanced)) {
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
	} else if (tt_time_compare(*root, tst, tet, is_start) == 1) {
		if (tt_insert_node(&(*root)->right, tst, tet, is_start,
					unbalanced)) {
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
	} else {
		(*root)->freq += 1;
		*unbalanced = 0;
	}
	ret = 0;
out:
	return ret;
}

void tt_avl_lvr(struct avl_node *root)
{
	if (root) {
		tt_avl_lvr(root->left);
		printf("%d:%d -- %d:%d\n", root->start_time->start_hour,
				root->start_time->start_min, root->end_time->end_hour,
				root->end_time->end_min);
		tt_avl_lvr(root->right);
	}
}

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

/*
struct avl_node *modified_avl_search(struct avl_node *tree, int hour, int min)
{
	struct avl_node *node = NULL;	

	if (!tree)
		return NULL;

	node = modified_avl_search(tree->left, hour, min);
	if (node && node->color == 0) {
		if (modified_compare(node, hour, min) >= 0) {
			node->color = 1;
		}
	} else if (!node && tree->color == 0) {
//	} else if (tree->color == 0) {
		node = NULL;
		if (modified_compare(tree, hour, min) >= 0) {
			tree->color = 1;
			node = tree;
		}
	}
	if (!node) {
		node = modified_avl_search(tree->right, hour, min);
		if (node && node->color == 0) {
			if (modified_compare(node, hour, min) >= 0) {
				node->color = 1;
			}
		}
	}

	return node;
}
*/

struct avl_node *modified_avl_search(struct avl_node *tree, int hour, int min)
{
	struct avl_node *node = NULL;	

	if (!tree)
		return NULL;

	if (tree->color == 0) {
		node = modified_avl_search(tree->left, hour, min);
	}
	if (!node && tree->color == 0) {
		if (modified_compare(tree, hour, min) >= 0) {
			tree->freq -= 1;
			if (!tree->freq)
				tree->color = 1;
			node = tree;
		}
	}
	if (!node) {
		node = modified_avl_search(tree->right, hour, min);
	}

	return node;
}

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
