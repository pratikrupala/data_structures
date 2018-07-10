#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
find_number(int a[], int size)
{
	int xor = 0;
	int actual_xor = 0;
	int i = 0;

	for (i = 1; i <= size + 1; i++) {
		xor ^= i;
	}

	for (i = 0; i < size; i++) {
		actual_xor ^= a[i];
	}

	printf("\nMissing number: %d\n", (xor ^ actual_xor));
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
