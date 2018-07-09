#include <stdio.h>
#include "qsort.h"

void
find_pair(int a[], int size, int n)
{
	int l = 0;
	int r = size - 1;
	int found = 0;

	my_qsort(a, 0, size - 1);

	while (l < r) {
		if (a[l] + a[r] == n) {
			found = 1;
			break;
		}
		if (a[l] + a[r] > n) {
			r--;
		} else {
			l++;
		}
	}
	if (found) {
		printf("\nPair: %d and %d for %d\n", a[l], a[r], n);
	} else {
		printf("\nNo pair found for %d\n", n);
	}
}

int
main(void)
{
	int a[8] = {23, 6, -2, 7, 123, 7, 8, 0};
	int size = sizeof(a)/sizeof(a[0]);
	int n = -1;
	int i = 0;

	printf("\nArray: ");
	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	printf("Enter a number for which a pair needs to be found: ");
	scanf("%d", &n);
	find_pair(a, size, n);

	return 0;
}
