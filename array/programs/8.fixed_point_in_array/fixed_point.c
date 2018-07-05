#include <stdio.h>

int
find_fixed_point(int a[], int start, int end, int size)
{
	int middle = (start + end) / 2;
	if (end >= start) {
		if (middle == a[middle]) {
			return middle;
		}

		if (middle > a[middle]) {
			if (middle != size - 1) {
				return find_fixed_point(a, middle + 1, end, size);
			} else {
				return -1;
			}
		} else {
			if (middle != 0) {
				return find_fixed_point(a, start, middle - 1, size);
			} else {
				return -1;
			}
		}
	}

	return -1;
}

int
main(void)
{
	//int a[8] = {-12, -4, 0, 3, 5, 8, 11, 34};
	//int a[8] = {-12, -4, 0, 1, 2, 5, 11, 34};
	//int a[8] = {-12, -4, 0, 1, 2, 3, 6, 34};
	//int a[8] = {-12, -4, 2, 5, 8, 9, 10, 34};
	int a[8] = {0, 2, 5, 8, 9, 10, 11, 34};
	int size = sizeof(a)/sizeof(a[0]);
	int result = -1;
	int i = 0;

	printf("\nArray: ");
	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	result = find_fixed_point(a, 0, size - 1, size);
	if (result == -1) {
		printf("\nFailed to find the fixed point in the array.\n");
	} else {
		printf("\nFixed point: %d\n", result);
	}

	return 0;
}
