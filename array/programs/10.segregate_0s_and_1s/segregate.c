#include <stdio.h>

#define SWAP(a, b, t) ((t = a), (a = b), (b = t))

void
segregate(int a[], int size)
{
	int l = 0;
	int tmp = 0;
	int r = size - 1;

	while (l < r) {
		while ((a[l] == 0) && (l <= size - 1)) {
			l++;
		}

		while ((a[r] == 1) && (r >= 0)) {
			r--;
		}

		if (l < r) {
			SWAP(a[l], a[r], tmp);
		}
	}
}

int
main(void)
{
	//int a[8] = {0, 1, 1, 0, 0, 0, 1, 1};
	//int a[2] = {0, 1};
	//int a[2] = {1, 0};
	//int a[2] = {0, 0};
	int a[8] = {1, 1, 1, 1, 1, 1, 1, 0};
	int size = sizeof(a)/sizeof(a[0]);
	int i = 0;

	printf("\nArray: ");
	for (i =0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	segregate(a, size);

	printf("\nResult: ");
	for (i =0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	return 0;
}
