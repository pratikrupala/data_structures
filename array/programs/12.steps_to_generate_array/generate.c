#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void
count_steps(int a[], int size)
{
	int i;
	int count = 0;
	int allzero = 0;

	while (1) {
		/* Make all the odd numbers even in this
		 * iteration
		 */
		for (i = 0; i < size; i++) {
			if ((a[i] != 0) && (a[i] % 2)) {
				a[i] = a[i] - 1;
				count++;
				if (a[i] == 0)
					allzero++;
			}
		}
		/* If all numbers have become zero, the array
		 * is empty and we reached to the end
		 */
		if (allzero == size)
			break;
		/* Devide all numbers by 2 once they all are
		 * even
		 */
		for (i = 0; i < size; i++) {
			a[i] /= 2;
		}
		count++;
		
	}
	printf("\nResult: %d\n", count);
}

int
main(void)
{
	int size = 0;
	int *a = NULL;
	int i = 0;

	printf("Enter the size of target array: ");
	scanf("%d", &size);

	a = malloc(size * sizeof(int));
	memset(a, 0, size);

	printf("\nEnter the array elements: \n");
	for (i = 0; i < size; i++) {
		printf("Index[%d]: ", i);
		scanf("%d", &(a[i]));
	}
	printf("\n");

	printf("\nTarget array: ");
	for (i = 0; i< size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	count_steps(a, size);

	return 0;
}
