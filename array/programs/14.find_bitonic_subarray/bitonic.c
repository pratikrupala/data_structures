#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
find_bitonic(int a[], int size)
{
	int inc[size], dec[size];
	int i = 0;
	int result = -1;

	memset(inc, 0, size);
	memset(dec, 0, size);
	inc[0] = 1;
	dec[size - 1] = 1;

	for (i = 1; i < size; i++) {
		if (a[i] > a[i - 1]) {
			inc[i] = inc[i - 1] + 1;
		} else {
			inc[i] = 1;
		}
	}

	for (i = size - 2; i >= 0; i--) {
		if (a[i] > a[i + 1]) {
			dec[i] = dec[i + 1] + 1;
		} else {
			dec[i] = 1;
		}
	}

	result = inc[0] + dec[0] - 1;

	for (i = 1; i < size; i++) {
		if ((inc[i] + dec[i] - 1) > result) {
			result = inc[i] + dec[i] - 1;
		}
	}

	printf("\nResult: %d\n", result);
}

int
main(void)
{
//	int a[10] = {3, 5, 2, 56, 2, 6, 5, 11, -22, 123};
	int *a = NULL;
	int size = -1;
	int i = 0;

	printf("\nEnter the size of the array: ");
	scanf("%d", &size);

	a = malloc(size * sizeof(int));
	if (!a) {
		printf("Failed to allocate memory for an array.\n");
		return -1;
	}

	printf("\nEnter array elements:\n");
	for (i = 0; i < size; i++) {
		printf("Index[%d]: ", i);
		scanf("%d", &(a[i]));
	}
	printf("\n");

	printf("\nArray: ");
	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	find_bitonic(a, size);

	return 0;
}
