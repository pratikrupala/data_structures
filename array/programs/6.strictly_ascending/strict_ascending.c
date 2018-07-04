#include <stdio.h>

int
make_ascending(int a[], int size)
{
	int i = 0;
	int j = 0;

	if (size == 1) {
		printf("Already sorted.\n\n");
		return -1;
	}

	/* Bring the i to the point where array is detected to be
	 * unsorted from the front side 
	 */
	for (i = 1; i < size; i++) {
		if (a[i] <= a[i - 1]) {
			break;
		}
	}

	if (i == size - 1) {
		printf("Already sorted.\n\n");
		return -1;
	}

	/* Bring j to the point in the array from where array starts
	 * becoming non decreasing from the back of the array.
	 * But let j go upto i - 1 only. Because from there we can
	 * possibility of making it sorted.
	 */
	for (j = size - 1; j >= 1; j--) {
		if (a[j] < a[j - 1]) {
			j--;
			break;
		}
	}

	if (i > j) {
		if (j == 0) {
			/* If j has reached at first element, then just need to take
			 * care of one number.
			 */
			a[j] = a[j + 1] - 1;
		} else if (i == size - 1) {
			/* If j has reached at first element, then just need to take
			 * care of one number.
			 */
			a[i] = a[i - 1] + 1;
		} else if ((a[i - 1] < a[i + 1]) ) {
			a[i] = (a[i - 1] + a[i + 1]) / 2;
		} else if (a[j - 1] < a[j + 1]) {
			a[j] = (a[j - 1] + a[j + 1]) / 2;
		} else {
			printf("Not Possible.\n\n");
			return -1;
		}
	} else {
		/* If i and j don't cross, then they are too much apart.
		 * So no possiblity of making it sorted by just changing
		 * one number.
		 */
		printf("Not Possible.\n\n");
		return -1;
	}

	return 0;
}

int
main(void)
{
	//int a[9] = {3, 3, 5, 8, 9, 333, 14, 15, 17};
        //int a[6] = {1, 4, 5, 1, 2, 3};
        //int a[4] = {-1, 4, 2, 3};
	//int a[6] = {2, 4, 5, 7, -3, -2};
	//int a[6] = {-22, -44, 5, 7, 9, 12};
	//int a[6] = {-66, -44, 5, 7, 12, 9};
	//int a[6] = {-66, -44, 5, 7, 12, 12};
	int a[5] = {1, 2, 5, 3, 3};
	int size = sizeof(a)/sizeof(a[0]);
	int result = -1;
	int i = 0;

	printf("\nArray: ");
	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	result = make_ascending(a, size);

	if (!result) {
		printf("\nNew Array: ");
		for (i = 0; i < size; i++) {
			printf("%d ", a[i]);
		}
		printf("\n\n");
	}

	return 0;
}
