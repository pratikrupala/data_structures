#include <stdio.h>
#include <stdlib.h>

void
spiral_print(int **a, int row, int col)
{
	int row_start = 0;
	int row_end = row - 1;
	int col_start = 0;
	int col_end = col - 1;
	int total = 0;
	int i = 0;

	printf("You have entered: \n");
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			printf("%d\t", a[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	while (total != (row * col)) {
		for (i = col_start; i <= col_end; i++) {
			printf("%d\t", a[row_start][i]);
			total++;
		}
		row_start++;
		for (i = row_start; i <= row_end; i++) {
			printf("%d\t", a[i][col_end]);
			total++;
		}
		col_end--;
		for (i = col_end; i >= col_start; i--) {
			printf("%d\t", a[row_end][i]);
			total++;
		}
		row_end--;
		for (i = row_end; i >= row_start; i--) {
			printf("%d\t", a[i][col_start]);
			total++;
		}
		col_start++;
	}
	printf("\n");
	printf("\n");
}

int
main(void)
{
	int i = 0;
	int **a = NULL;
	int row = -1;
	int col = -1;

	printf("Enter the number of rows: ");
	scanf("%d", &row);

	printf("Enter the number of cols: ");
	scanf("%d", &col);

	if ((row < 1) || (col < 1)) {
		printf("Invalid dimentions.\n");
		return -1;
	}

	a = malloc(row * sizeof(int *));

	for (i = 0; i < row; i++) {
		a[i] = malloc(col * sizeof(int));
	}

	printf("Enter the values for the elements: \n");
	for (i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			printf("a[%d][%d]: ", i, j);
			scanf("%d", &(a[i][j]));
		}
	}

	spiral_print(a, row, col);

	/*
	printf("You have entered: \n");
	for (i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			printf("%d\t", a[i][j]);
		}
		printf("\n");
	}
	printf("\n");
*/
	return 0;
}
