#include <stdio.h>
#include <string.h>

void
search_span(int a[], int b[], int size)
{
	int aux_len = (2 * size) + 1;
	int aux[aux_len];
	int i = 0;
	int sum_a = 0;
	int sum_b = 0;
	int sum_diff = 0;
	int span = -1;

	memset(aux, -1, sizeof(aux));

	for (i = 0; i < size; i++) {
		sum_a += a[i];
		sum_b += b[i];
		sum_diff = size + (sum_a - sum_b);

		if (!(sum_a - sum_b)) {
			span = i + 1;
		} else if (aux[sum_diff] == -1) {
			aux[sum_diff] = i;
		} else {
			if ((i - aux[sum_diff]) > span) {
				printf("Older span: %d\n", span);
				span = i - aux[sum_diff];
				printf("New span: %d, i: %d, aux[sum_diff]: %d\n", span, i, aux[sum_diff]);
			}
		}
	}

	printf("\nLongest span: %d\n", span);
}

int
main(void)
{
	/* data set 1
	int a[8] = {1, 0, 1, 1, 0, 0, 1, 0};
	int b[8] = {0, 0, 1, 0, 1, 1, 0, 0};
	*/
	/*
	int a[8] = {1, 0, 1, 1, 0, 0, 1, 0};
	int b[8] = {0, 0, 1, 0, 1, 1, 0, 1};
	*/
	/*
	int a[8] = {0, 1, 1, 1, 0, 0, 1, 0};
	int b[8] = {0, 0, 1, 0, 1, 1, 0, 0};
	*/
	int a[9] = {0, 1, 1, 1, 0, 0, 1, 0, 0};
	int b[9] = {0, 0, 1, 0, 1, 1, 0, 0, 0};
	int size = sizeof(a)/sizeof(a[0]);
	int i = 0;

	printf("\nArray[1]: ");
	for (i = 0; i < size; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	printf("\nArray[2]: ");
	for (i = 0; i < size; i++) {
		printf("%d ", b[i]);
	}
	printf("\n\n");
	
	search_span(a, b, size);

	return 0;
}
