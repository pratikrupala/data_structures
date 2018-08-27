#include <stdio.h>
#include <stdlib.h>

int
search_pivot(int *a, int left, int right)
{
	int m;

	if (left > right)
		return -1;

	if (left == right)
		return left;

	m = (left + right)/2;

	if ((m < right) && (a[m] > a[m + 1]))
		return m;
	if ((m > left) && (a[m] < a[m - 1]))
		return (m - 1);
	if (a[left] >= a[m])
		return search_pivot(a, left, m - 1);

	return search_pivot(a, m + 1, right);
}

int
search_key(int *a, int left, int right, int key)
{
	int m;

	if (left > right) 
		return -1;

	m = (left + right)/2;

	if (key == a[m]) {
		return m;
	}
	if (key > a[m]) {
		return search_key(a, m + 1, right, key);
	} else {
		return search_key(a, left, m - 1, key);
	}
}

int
search_rotated(int *a, int n, int key)
{
	int pivot = search_pivot(a, 0, n - 1);
	printf("\nPivot: %d\n", pivot);
	
	if (pivot == -1) {
		printf("\nArray is not rotated at all.\n");
		return search_key(a, 0, n - 1, key);
	}

	if (key <= a[n - 1]) {
		return search_key(a, pivot + 1, n - 1, key);
	} else {
		return search_key(a, 0, pivot, key);
	}
}

int
main(void)
{
	int len = -1;
	int i = 0;
	int *a = NULL;
	int key = -1;
	int result = -1;

	printf("Enter the length of the array: ");
	scanf("%d", &len);
	if (len < 1) {
		printf("\nInvalid length\n");
		return -1;
	}

	a = malloc(len * sizeof(int));
	if (!a) {
		printf("Failed to allocate memory.\n");
		return -1;
	}

	printf("Enter the values in array: \n");
	for (i = 0; i < len; i++) {
		printf("a[%d]: ", i);
		scanf("%d", &a[i]);
	}

	printf("Enter the number to be searched: ");
	scanf("%d", &key);

	result = search_rotated(a, len, key);
	printf("\nResult: %d\n", result);

	return 0;
}
