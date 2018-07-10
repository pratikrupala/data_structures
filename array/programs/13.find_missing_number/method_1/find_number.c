#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
find_number(int a[], int size)
{
	int sum = 0;
	int actual_sum = 0;
	int i = 0;

	sum = ((size + 1) * (size + 2)) / 2;

	for (i = 0; i < size; i++) {
		actual_sum += a[i];
	}

	printf("\nMissing number: %d\n", (sum - actual_sum));
}

int
main(void)
{
	int size = -1;
	int *a = NULL;
	int i = 0;

	printf("\nEnter the length of the number: ");
	scanf("%d", &size);

	a = malloc(size * sizeof(int));
	if (!a) {
		printf("Failed to allocate memory for the array.\n");
		return -1;
	}

	memset(a, 0, size);

	printf("\nEnter array elements: \n");
	for (i = 0; i < size; i++) {
		printf("Index[%d]: ", i + 1);
		scanf("%d", &(a[i]));
	}

	printf("\nArray: \n");
	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	find_number(a, size);

	return 0;
}
