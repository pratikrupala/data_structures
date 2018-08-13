#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
merge_span(int row, int col, int **a)
{
	int i = 0;
	int k = 0;

	for (i = 1, k = 0; i < row; i++) {
		if (a[i][0] < a[k][1]) {
			if (a[i][1] < a[k][1]) {
				/* Nothing to do.
				 * Just skip element at i
				 * as it is already covered
				 * by k.
				 */
				a[i][0] = -1;
				a[i][1] = -1;
			} else {
				a[k][1] = a[i][1];
				a[i][0] = -1;
				a[i][1] = -1;
			}
		} else if (a[k + 1][0] == -1) {
			k++;
			a[k][0] = a[i][0];
			a[k][1] = a[i][1];
			a[i][0] = -1;
			a[i][1] = -1;
		} else {
			k++;
		}
	}
	printf("\nDONE, k = %d, i = %d\n", k, i);
	i--;
	if (((i - k) > 1) && (a[i][0] != -1) && (a[k + 1][0] == -1)) {
		k++;
		printf("\nInner k = %d\n", k);
		a[k][0] = a[i][0];
		a[k][1] = a[i][1];
	}
	printf("\nReturning k: %d\n", k + 1);
	return (k + 1);
}

int
main(void)
{
	int **a = NULL;
	int row = -1;
	int col = -1;
	int i = 0;
	int j = 0;
	int k = 0;
	int p = 0;
	int r = 0;

	printf("Enter dimentions of the array:\n");
	printf("Number of rows: ");
	scanf("%d", &row);
	printf("Number of columns: ");
	scanf("%d", &col);

	a = malloc((row) * sizeof(int *));
	if (!a) {
		printf("Couldn't allocate memory for the array.\n");
		return -1;
	}

	for (p = 0; p < row; p++) {
		a[p] = malloc(col * sizeof(int));
		if (!a[p]) {
			printf("Couldn't allocate memory for the array.\n");
			return -1;
		}
	}

	printf("Enter element values: \n");
	for (i = 0; i < (row); i++) {
		for (j = 0; j < col; j++) {
			printf("Index[%d][%d]: ", i, j);
			scanf("%d", &(a[i][j]));
		}
	}

	printf("\nYour entered array: \n");
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++, k++) {
			printf("%d ", a[i][j]);
			if (!((k + 1) % col)) {
				printf("\n");
			}
		}
	}

	r = merge_span(row, col, a);

	printf("result: \n");
	for (i = 0; i < r; i++) {
		for (j = 0; j < col; j++, k++) {
			printf("%d ", a[i][j]);
			if (!((k + 1) % col)) {
				printf("\n");
			}
		}
	}

	return 0;
}
