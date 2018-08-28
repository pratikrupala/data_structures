#include <stdio.h>
#include <stdlib.h>

int
search_key(int *a, int left, int right, int key)
{
	int mid;

	if (left > right)
		return -1;

	mid = (left + right) / 2;
	if (key == a[mid]) {
		return mid;
	}

	if (a[left] <= a[mid]) {
		if (key >= a[left] && key <= a[mid]) {
			return search_key(a, left, mid - 1, key);
		}
		return search_key(a, mid + 1, right, key);
	}

	if (key >= a[mid] && key <= a[right]) {
		return search_key(a, mid + 1, right, key);
	}
	return search_key(a, left, mid - 1, key);
}

int
main(void)
{
	int *a = NULL;
	int len = -1;
	int key = -1;
	int result = -1;
	int i = 0;

	printf("Enter the length of the array: \n");
	scanf("%d", &len);
	if (len < 1) {
		printf("\nInvalid length of the array.\n");
		return -1;
	}

	a = malloc(len * sizeof(int));
	if (!a) {
		printf("Failed to allocate memory.\n");
		return -1;
	}

	printf("\nEnter array elements: \n");
	for (i = 0; i < len; i++) {
		printf("a[%d]: ", i);
		scanf("%d", &a[i]);
	}

	printf("enter the key to be searched: ");
	scanf("%d", &key);

	result = search_key(a, 0, len - 1, key);
	printf("\nResult: %d\n", result);

	return 0;
}
