#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
count_strictly_increasing_subarray(int a[], int size)
{
	int i = 0;
	int count = 1;
	int total = 0;

	for (i = 1; i < size; i++) {
		if (a[i] == (a[i - 1] + 1)) {
			count++;
		} else if(count > 1) {
			total += ((count) * (count - 1)) / 2;
			count = 1;
		}
	}
	if (count > 1) {
		total += ((count) * (count - 1)) / 2;
	}

	printf("\nTotal subarrays: %d\n", total);
}

/*
 * Tries test cases:
 * {1, 2, 3, 56, 789} - Result = 3
 * {34, 56, 1, 2, 3} -	Result = 3
 * {1, 2, 3}	-	Result = 3
 * {1, 2, 3, 4}	-	Result = 6
 * {1, 2, 3, 5, 6} -	Result = 4
 * {1, 2, 3, 5, 6, 4, 5, 6} -	Result = 7 
 */
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
		printf("\nFailed to allocate memory for the array.\n");
		return -1;
	}

	printf("\nEnter array elements: \n");
	for (i = 0; i < size; i++) {
		printf("i[%d]: ", i);
		scanf("%d", &(a[i]));
	}

	count_strictly_increasing_subarray(a, size);

	return 0;
}
