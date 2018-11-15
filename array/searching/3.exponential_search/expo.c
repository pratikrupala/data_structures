#include <stdio.h>
#include <stdlib.h>

#define min(a, b) ((a > b)?b:a)

int bin_search(int *arr, int low, int high, int n)
{
	int mid = (low + high)/2;

	if (low > high)
		return -1;

	if (arr[mid] == n)
		return mid;

	if (n > arr[mid])
		return (bin_search(arr, mid + 1, high, n));
	else
		return (bin_search(arr, low, mid - 1, n));
}

int exponential_search(int *arr, int n, int len)
{
	int j = 1;
	int ret = -1;

	while (j < len && n > arr[j])
		j *= 2;

	ret = bin_search(arr, j/2, min(j, len), n);

	return ret;
}

int main(void)
{
	int *arr = NULL;
	int len = -1;
	int i = 0;
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

	printf("\nEnter array elements in sorted order:\n");
	for (i = 0; i < len; i++) {
		printf("%d: ", i);
		scanf("%d", &arr[i]);
	}

	printf("\nYou have entered:\n");
	for (i = 0; i < len; i++) {
		printf("%d\t", arr[i]);
	}

	printf("\nEnter the number to be searched: ");
	scanf("%d", &n);

	ret = exponential_search(arr, n, len);
	if (ret == -1) {
		printf("\nNo such number is present in the array.\n");
	} else {
		printf("\n%d is present at index %d.\n", n, ret);
	}

	return 0;
}
