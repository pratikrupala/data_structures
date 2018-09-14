#include <stdio.h>
#include <stdlib.h>
#include <tree.h>

int find_index(int *a, int val, int s)
{
	int i = 0;

	while (i < s) {
		if (val == a[i])
			return i;
		i++;
	}

	return -1;
}

void generate_post_order(int *in, int *pre, int n)
{
	int root = -1;

	root = find_index(in, pre[0], n);

	if (root != 0)
		generate_post_order(in, pre + 1, root);

	if (root != n - 1)
		generate_post_order(in+root+1, pre+root+1, n-root-1);

	printf("%d\t", pre[0]);
}

int main(void)
{
	int inorder[6] = {4, 2, 5, 1, 6, 3};
	int preorder[6] = {1, 2, 4, 5, 3, 6};
	int size = sizeof(inorder)/sizeof(inorder[0]);
	
	generate_post_order(inorder, preorder, size);
	printf("\n");

	return 0;
}
