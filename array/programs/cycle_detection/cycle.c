#include <stdio.h>

void
detect_cycle(int a[], int size)
{
	int slow = 0;
	int fast = 0;
	int lamda = 0;
	int mu = 0;
	int pow = 0;
	int i = 0;

	slow = a[0];
	fast = a[slow];
	pow = 1;
	lamda = 1;
	while (slow != fast) {
		if (pow == lamda) {
			lamda = 0;
			pow = pow * 2;
			slow = fast;
		}
		fast = a[fast];
		lamda += 1;
	}

	fast = a[0];
	for (i = 0; i < lamda; i++) {
		fast = a[fast];
	}
	slow = a[0];
	mu = 0;
	while (slow != fast) {
		slow = a[slow];
		fast = a[fast];
		mu += 1;
	}


	printf("\n\nResult: Mu = %d | Lamda = %d\n", mu, lamda);
}

int
main(void)
{
	int a[9] = {3, 7, 8, 4, 6, 2, 0, 1, 5};
	int size = sizeof(a)/sizeof(a[0]);
	int i = 0;

	printf("\n");
	printf("Original array:\n");
	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}

	detect_cycle(a, size);

	return 0;
}
