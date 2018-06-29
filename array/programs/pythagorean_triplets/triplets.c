#include <stdio.h>
#include "qsort.h"

void find_triplets(int a[], int size)
{
	int i = 0;
	int check = 0;
	int first = 0;
	int second = 0;

	// Square all the numbers
	for (i = 0; i < size; i++) {
		a[i] = a[i] * a[i];
	}

	// Use quick sort implemented in qsort.h in ../../headers/ directory
	my_qsort(a, 0, size - 1);

	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	// Search the triplet
	for (i = size - 1; i > 1; i--) {
		check = a[i];
		first = 0;
		second = i - 1;
		while (first < second) {
			if (check == (a[first] + a[second])) {
				printf("Found the triplet.\n");
				printf("First: %d, second: %d\n", first, second);
				printf("Triplet: %d + %d = %d\n", a[first], a[second], check);
				break;
			} else if (check < (a[first] + a[second])) {
				second--;
			} else if (check > (a[first] + a[second])) {
				first++;
			}
		}
	}
}

int main(void)
{
	int a[9] = {5, 2, 9, 22, 1, 4, 13, 3, 353};
	int size = sizeof(a)/sizeof(a[0]);

	find_triplets(a, size);

	return 0;
}
