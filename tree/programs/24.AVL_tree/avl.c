#include <stdio.h>
#include <stdlib.h>

struct avl_q {
	struct avl *node;
	struct avl_q *next;
};

struct avl_q *front = NULL;
struct avl_q *rear = NULL;

struct avl {
	int a;
	int bf;
	char s;
	int parent;
	struct avl *left;
	struct avl *right;
};

void left_rotate(struct avl **root, int *unbalanced)
{
	struct avl *child = (*root)->left;
	struct avl *grandchild;
	if (child->bf == 1) {
		(*root)->left = child->right;
		child->right = *root;
		(*root)->bf = 0;
		*root = child;
	} else {
		grandchild = child->right;
		(*root)->left = grandchild->right;
		grandchild->right = *root;
		child->right = grandchild->left;
		grandchild->left = child;
		switch(grandchild->bf) {
			case -1:
				(*root)->bf = 0;
				child->bf = 1;
				break;
			case 0:
				(*root)->bf = child->bf = 0;
				break;
			case 1:
				(*root)->bf = -1;
				child->bf = 0;
		}
		*root = grandchild;
	}
	
	(*root)->bf = 0;
	*unbalanced = 0;
}

void right_rotate(struct avl **root, int *unbalanced)
{
	struct avl *child = (*root)->right;
	struct avl *grandchild;
	if (child->bf == -1) {
		(*root)->right = child->left;
		child->left = *root;
		(*root)->bf = 0;
		*root = child;
	} else {
		grandchild = child->left;
		(*root)->right = grandchild->left;
		grandchild->left = *root;
		child->left = grandchild->right;
		grandchild->right = child;
		switch(grandchild->bf) {
			case 1:
				(*root)->bf = 0;
				child->bf = 1;
				break;
			case 0:
				(*root)->bf = child->bf = 0;
				break;
			case -1:
				(*root)->bf = -1;
				child->bf = 0;
		}
		*root = grandchild;
	}
	(*root)->bf = 0;
	*unbalanced = 0;
}

int avl_insert(struct avl **root, int key, int *unbalanced)
{
	struct avl *node = NULL;
	int ret = -1;
	*unbalanced = 1;
	if (!*root) {
		node = malloc(sizeof(struct avl));
		if (!node)
			goto out;
		node->a = key;
		node->bf = 0;
		node->left = node->right = NULL;
		*root = node;
		*unbalanced = 0;
	} else if (key < (*root)->a) {
		if (avl_insert(&(*root)->left, key, unbalanced))
			goto out;
		switch ((*root)->bf) {
			case -1:
				(*root)->bf = 0;
				break;
			case 0:
				(*root)->bf = 1;
				break;
			case 1:
				left_rotate(root, unbalanced);
				break;
			default:
				printf("\nInvalid case in left subtree.\n");
		}
	} else if (key > (*root)->a) {
		if (avl_insert(&(*root)->right, key, unbalanced))
			goto out;
		switch ((*root)->bf) {
			case 1:
				(*root)->bf = 0;
				break;
			case 0:
				(*root)->bf = -1;
				break;
			case -1:
				right_rotate(root, unbalanced);
				break;
			default:
				printf("\nInvalid case in right subtree.\n");
		}
	} else {
		printf("\nKey %d already exists. Aborting.\n", key);
		goto out;
	}
	ret = 0;

out:
	return ret;
}

int avl_enq(struct avl *n)
{
	int ret = -1;
	struct avl_q *ptr = malloc(sizeof(struct avl_q));
	if (!ptr)
		goto out;
	ptr->node = n;
	ptr->next = NULL;

	if (!front) {
		front = rear = ptr;
	} else {
		rear->next = ptr;
		rear = rear->next;
	}
	ret = 0;
out:
	return ret;
}

struct avl *avl_dq(void)
{
	struct avl *ptr = NULL;
	struct avl_q *qptr = front;
	if (qptr) {
		ptr = front->node;
		front = front->next;
		free(qptr);
	}

	return ptr;
}

void avl_level_order(struct avl *root)
{
	int i = 0;
	int child_count = 1;

	if (!root) 
		return;

	root->s = 'M';
	root->parent = 0;
	if (avl_enq(root)) {
		printf("\nFailed to enqueue %d\n", root->a);
		return;
	}
	while (root) {
		i = child_count;
		child_count = 0;

		if (!i)
			break;

		while (i) {
			root = avl_dq();
			printf("%d(%d/%c/%d)\t", root->a, root->bf, root->s,
				root->parent);
			i--;

			if (root->left) {
				root->left->s = 'L';
				root->left->parent = root->a;
				avl_enq(root->left);
				child_count++;
			}
			if (root->right) {
				root->right->s = 'R';
				root->right->parent = root->a;
				avl_enq(root->right);
				child_count++;
			}
		}
		printf("\n");
	}
}

int main(void)
{
	struct avl *root = NULL;
	int n = -1;
	int k = -1;
	int i = 0;
	int unbalanced = 0;

	printf("\nEnter the number of nodes to be entered in AVL tree: ");
	scanf("%d", &n);
	if (n < 1) {
		printf("\nInvalid number of nodes in tree.\n");
		goto out;
	}

	printf("\nEnter the numbers to be added:\n");
	for (i = 0; i < n; i++) {
		printf("[%d]: ", i);
		scanf("%d", &k);
		if (avl_insert(&root, k, &unbalanced)) {
			printf("\nAVL insert failed for %d\n", k);
			goto out;
		}
		k = -1;
	}

	printf("\n");
	avl_level_order(root);

out:
	return 0;
}
