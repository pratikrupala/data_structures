#include <stdio.h>

#define SWAP(a, b, t) ((t = a), (a = b), (b = t))

int do_partition(int a[], int start, int end)
{
	int pivot = end;
	int limit = start - 1;
	int forward = start;
	int tmp = 0;

	for (forward = start; forward < end; forward++) {
		if ((a[pivot] >= a[forward])) {
			limit++;
			SWAP(a[forward], a[limit], tmp);
		}
	}

//	printf("a[limit]: %d\n", a[limit]);
	SWAP(a[pivot], a[limit + 1], tmp);
	return limit + 1;
}

void sort_it(int a[], int start, int end)
{
	if (start < end) {
		int pivot = do_partition(a, start, end);
		printf("Pivot: %d\n", pivot);

		printf("DEBUG LEFT: pivot = %d, start = %d\n", pivot, start);
		sort_it(a, start, pivot - 1);

		printf("DEBUG RIGHT: pivot = %d, end = %d\n", pivot, end);
		sort_it(a, pivot + 1, end);
	}
}

int main(void)
{
	//int a[10] = {23, -54, 5, 0, 2, 44, 143, 8, 43, 98};
	int a[5] = {23, -54, 5, 0, 2};
	int size = sizeof(a)/sizeof(a[0]);
	int i = 0;

	printf("\t");
	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
	sort_it(a, 0, size - 1);
	printf("\n");
	printf("\t");
	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
	return 0;
}
