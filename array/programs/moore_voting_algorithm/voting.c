#include <stdio.h>

int
vote(int a[], int size)
{
	int count = 1;
	int prev = 0;
	int candidate = 0;
	int i = 0;

	for (i = 1; i < size; i++) {
		if (a[i] == a[prev]) {
			count++;
		} else {
			count--;
			if (!count) {
				prev = i;
				count = 1;
			}
		}
	}

	candidate = a[prev];
	count = 0;
	for (i = 0; i < size; i++) {
		if (a[i] == candidate)
			count++;
	}

	if (count >= (size / 2))
		return candidate;
	else
		return -1;
}

int
main(void)
{
	//int a[14] = {2, 3, 2, 4, 2, 4, 2, 1, 2, 33, 2, 23, 2, 67};
	int a[14] = {2, 3, 2, 4, 2, 5, 2, 1, 2, 33, 2, 23, 2, 67};
	int size = sizeof(a)/sizeof(a[0]);
	int result = -1;
		
	result = vote(a, size);
	if (result == -1) {
		printf("No majority element.\n");
	} else {
		printf("Majority element: %d\n", result);
	}

	return 0;
}
