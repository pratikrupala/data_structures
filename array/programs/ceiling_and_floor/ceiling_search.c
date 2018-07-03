#include <stdio.h>

int
search_floor(int a[], int size, int x)
{
	int i = 0;

	if (x > a[size - 1]) {
		return a[size - 1];
	}

	if (x < a[0]) {
		return -1;
	}

	for (i = (size - 1); i >= 1; i--) {
		if (x > a[i - 1])
			return a[i - 1];
	}

	return -1;
}

int
search_ceil(int a[], int size, int x)
{
	int i = 0;

	if (x <= a[0]) {
		return a[0];
	}

	if (x > a[size - 1]) {
		return -1;
	}

	for (i = 1; i < size - 1; i++) {
		if (x <= a[i + 1])
			return a[i + 1];
	}

	return -1;
}

int
main(void)
{
	int a[8] = {2, 4, 6, 7, 11, 16, 45, 56};
	int size = sizeof(a)/sizeof(a[0]);
	int number = -1;
	int result = -1;
	int i = 0;

	printf("\nArray: ");
	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	// Number of which ceiling needs to be found
	printf("Enter input for ceiling: ");
	scanf("%d", &number);

	result = search_ceil(a, size, number);
	if (result == -1) {
		printf("\nNo ceiling present for input: %d\n", number);
	} else {
		printf("\nCeil value of %d in array is %d\n", number, result);
	}

	result = -1;

	printf("\nArray: ");
        for (i = 0; i < size; i++) {
                printf("%d ", a[i]);
        }
        printf("\n");

	// Number of which floor needs to be found
	printf("Enter input for floor: ");
	scanf("%d", &number);

	result = search_floor(a, size, number);
        if (result == -1) {
                printf("\nNo floor present for input: %d\n", number);
        } else {
                printf("\nFloor value of %d in array is %d\n", number, result);
        }
	return 0;
}
