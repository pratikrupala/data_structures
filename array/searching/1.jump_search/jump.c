#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int jump_search(int *arr, int len, int n)
{
	int last = 0;
	int jump = sqrt(len);

	while (arr[jump] < n) {
		jump += sqrt(n);
		if (jump > len) {
			return -1;
		}
	}

	while (arr[last] < n) {
		last++;
		if (arr[last] == n)
			return 0;
	}

	return -1;
}

int main(void)
{
	int i = 0;
	int n = -1;
	int *arr = NULL;
	int len = -1;
	int ret = -1;

	printf("\nEnter the length of the array: ");
	scanf("%d", &len);
	if (len < 1) {
		printf("\nInvalid length of the array.\n");
		return ret;
	}

	arr = malloc(len * sizeof(int));
	if (!arr) {
		perror("Malloc failed: ");
		return ret;
	}

	printf("\nEnter the array elements:\n");
	for (i = 0; i < len; i++) {
		printf("%d: ", i);
		scanf("%d", &arr[i]);
	}

	printf("\nYour entered array:\n");
	for (i = 0; i < len; i++) {
		printf("%d\t", arr[i]);
	}

	printf("\nEnter the number to be searched: ");
	scanf("%d", &n);
	
	ret = jump_search(arr, len, n);
	if (ret) {
		printf("\nNo such number preset in the array.\n");
		goto out;
	} else {
		printf("\n%d found in the array.\n", n);
	}

	ret = 0;
out:
	return ret;
}
