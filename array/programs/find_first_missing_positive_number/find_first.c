#include <stdio.h>

#define SWAP(m, n, p) ((p = m), (m = n), (n = p))

void find_first_num(int a[], int size)
{
	int i = 0;
	int t = 0;
	int num = 0;

	while (i < size) {
		t = 0;
		// Don't consider 0s, negative numbers and number beyond the range
		// 1 to n. If they are present, ignore them.
		if ((a[i] <= 0) || (a[i] > size)) {
			i++;
			continue;
		}
		num = a[i];
		// Ignore repititive numbers as well.
		if (a[i] == a[num - 1]) {
			i++;
			continue;
		}
		// Swap the numbers so that index number and element values match
		// with each other. In our case, array starts with 0, so 
		// relationship here should be : (index - 1) = value
		SWAP(a[i], a[num - 1], t);
	}

	for (i = 0; i < size; i++) {
		if ((a[i] <= 0) || (a[i] > size)) {
			// first 0 or negative or beyond the size element
			// represents missing number.
			printf("Missing number: %d\n", i + 1);
			break;
		}
	}
}

int main(void)
{
	int a[8] = {8, 4, -2, 1, 0, 6, 1, 3};
	int size = sizeof(a)/sizeof(a[0]);
	int i = 0;

	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	find_first_num(a, size);
	printf("done\n");

	return 0;
}
