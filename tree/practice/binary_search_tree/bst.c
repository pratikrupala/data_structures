#include <stdio.h>
#include <stdlib.h>

struct bst {
	int k;
	int lr;
	struct bst *left;
	struct bst *right;
};

struct cqueue {
	struct bst *node;
	struct cqueue *next;
};

struct cqueue *qfront = NULL;
struct cqueue *qrear = NULL;

int cq_enq(struct bst *node)
{
	int ret = -1;
	if (!qfront) {
		qfront = qrear = malloc(sizeof(struct cqueue));
		if (!qfront) {
			printf("\nFailed to allocate memory for queue.\n");
			goto out;
		}
		qfront->next = qrear->next = NULL;
		qfront->node = node;
	} else {
		qrear->next = malloc(sizeof(struct cqueue));
		if (!qrear->next) {
			printf("\nFailed to allocate memory for queue.\n");
			goto out;
		}
		qrear = qrear->next;
		qrear->node = node;
		qrear->next = NULL;
	}
	ret = 0;

out:
	return ret;
}

struct bst *cq_deque(void)
{
	struct cqueue *ptr = qfront;

	if (qfront) {
		qfront = qfront->next;
		return ptr->node;
	}
	return NULL;
}

struct bst *insertion_search(struct bst *root, int m) 
{
	struct bst *prev = root;
	while (root) {
		if (root->k == m) {
			printf("\nKey is already present.\n");
			goto out;
		} else if (root->k < m) {
			prev = root;
			root = root->right;
		} else {
			prev = root;
			root = root->left;
		}
	}
out:
	return prev;
}

int bst_insert(struct bst **root, int v)
{
	int ret = -1;
	struct bst *node = NULL;
	if (!*root) {
		*root = malloc(sizeof(struct bst));
		if (!*root) {
			printf("Failed to allocate memory for %d."
					" Aborting\n", v);
			goto insert_out;
		}
		(*root)->k = v;
		(*root)->lr = 0;
		(*root)->left = (*root)->right = NULL;
	} else {
		node = insertion_search(*root, v);
		if (node->k == v) {
			goto insert_out;
		} else if (node->k < v) {
			node->right = malloc(sizeof(struct bst));
			if (!node->right) {
				printf("\nFailed to allocate memory for"
						" %d. Aborting\n", v);
				goto insert_out;
			}
			node->right->k = v;
			node->right->lr = node->lr + 1;
		} else {
			node->left = malloc(sizeof(struct bst));
			if (!node->left) {
				printf("\nFailed to allocate memory for"
						" %d. Aborting\n", v);
				goto insert_out;
			}
			node->left->k = v;
			node->left->lr = node->lr - 1;
		}
	}
	ret = 0;
insert_out:
	return ret;
}

void level_order_bst(struct bst *root)
{
	int i = 0;
	int child_count = 1;

	if (!root) {
		printf("\nBST is empty.\n");
		return;
	}

	if (cq_enq(root) == -1) {
		printf("\nEnqueue operation failed. Aborting\n");
		return;
	}
	
	while (root) {
		i = child_count;
		child_count = 0;

		if (!i) {
			break;
		}

		while (i) {
			root = cq_deque();
			printf("%d(%d)\t", root->k, root->lr);
			free(root);
			i--;

			if (root->left) {
				if (cq_enq(root->left) == -1) {
					printf("\nFailed to enqueue child"
						" %d.\n", root->left->k);
					return;
				}
				child_count++;
			}
			if (root->right) {
				if (cq_enq(root->right) == -1) {
					printf("\nFailed to enqueue child"
						" %d.\n", root->right->k);
					return;
				}
				child_count++;
			}
			printf("\n\n");
		}
	}
}

int main(void)
{
	int i = 0;
	int n = -1;
	int a = -1;
	int ret = -1;
	struct bst *root = NULL;

	printf("Enter the number of elements for bst: ");
	scanf("%d", &n);
	if (n < 1) {
		printf("\nInvalid number of elements.\n");
		goto out;
	}

	printf("\nEnter the element numbers:\n");
	for (i = 0; i < n; i++) {
		printf("[%d]: ", i);
		scanf("%d", &a);
		ret = bst_insert(&root, a);
		if (ret)
			goto out;
		a = -1;
	}

	printf("\n");
	level_order_bst(root);
	printf("\n");
	ret = 0;

out:
	return ret;
}
