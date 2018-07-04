#include <stdio.h>

void
find_dist(int a[], int size, int first, int second)
{
	int i = 0;
	int old = 0;
	int dist = -1;

	for (i = 0; i < size; i++) {
		if (a[i] == first || a[i] == second) {
			old = i;
			break;
		}
	}

	for (; i < size; i++) {
		if (a[i] == first || a[i] == second) {
			if ((a[old] != a[i]) && (dist < (i - old))) {
				printf("i = %d, old = %d\n", i, old);
				dist = i - old;
			}
			old = i;
		}
	}

	printf("Min distance: %d\n", dist);
}

int
main(void)
{
	int a[9] = {4, 6, 2, 7, 8, 443, -2, 0, 23};
	//int a[2] = {0, 23};
	int size = sizeof(a)/sizeof(a[0]);
	int i = 0;
	int first = 0;
	int second = 0;

	printf("\n");
	printf("Array elements: \n");
	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	printf("Enter first number: ");
	scanf("%d", &first);

	printf("Enter second number: ");
	scanf("%d", &second);

	find_dist(a, size, first, second);

	return 0;
}
