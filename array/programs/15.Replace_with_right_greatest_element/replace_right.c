#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
replace_elements(int a[], int size)
{
	int i = 0;
	int max = -1;
	int latest = -1;

	for (i = size - 1; i >= 0; i--) {
		latest = a[i];
		a[i] = max;
		max = (latest > max) ? latest : max;
	}
}

int
main(void)
{
	int *a = NULL;
	int size = -1;
	int i = 0;

	printf("\nEnter the size of the array: ");
	scanf("%d", &size);

	a = malloc(size * sizeof(int));
	if (!a) {
		printf("Failed to allocate memory for the array.\n");
		return -1;
	}

	memset(a, 0, size);

	printf("\nEnter array elements: \n");
	for (i = 0; i < size; i++) {
		printf("index[%d]: ", i);
		scanf("%d", &(a[i]));
	}

	printf("\nArray: ");
	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	replace_elements(a, size);

	printf("\nReplaced array: ");
	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	return 0;
}
