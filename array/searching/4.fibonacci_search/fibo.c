#include <stdio.h>
#include <stdlib.h>

#define min(a, b) ((a<b)?a:b)

int fibo_search(int *arr, int len, int n)
{
	int i = 0;
	int f2 = 0;
	int f1 = 1;
	int f = f1 + f2;
	int off = -1;
	
	while (f < len) {
		f2 = f1;
		f1 = f;
		f = f1 + f2;
	}

	while (f > 1) {
		i = min(off + f2, len);

		if (arr[i] < n) {
			f = f1;
			f1 = f2;
			f2 = f - f1;
			off = i;
		} else if (arr[i] > n) {
			f = f2;
			f1 = f1 - f2;
			f2 = f - f1;
		} else {
			return i;
		}
	}

	if (f1 && arr[off + 1] == n) {
		return off + 1;
	}

	return -1;
}

int main(void)
{
	int i = 0;
	int *arr = NULL;
	int len = -1;
	int n = 0;
	int ret = -1;

	printf("\nEnter the length of the array: ");
	scanf("%d", &len);
	if (len < 2) {
		printf("\nInvalid length.\n");
		return -1;
	}

	arr = malloc(len * sizeof(int));
	if (!arr) {
		printf("\nFailed to allocate memory for array.\n");
		return -1;
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

	printf("\n\nEnter the number to be searched: ");
	scanf("%d", &n);

	ret = fibo_search(arr, len, n);
	if (ret == -1) {
		printf("\nCould not find %d in the array.\n", n);
	} else {
		printf("\nFound %d at index %d in the array.\n", n, ret);
	}

	return 0;
}
