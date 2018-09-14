#include <stdio.h>
#include <stdlib.h>

int* findDiagonalOrder(int** matrix, int matrixRowSize, int matrixColSize, int* returnSize) {
	*returnSize = matrixRowSize * matrixColSize;
	int *a = NULL;
	int i = -1;
	int j = 0;
	int k = 0;
	int count = 0;

	a = (int *) malloc(*returnSize * sizeof(int));
	if (!a) {
		printf("\nFailed to allocate memory for result.\n");
		return NULL;
	}

	count = *returnSize;
	while (count) {
		if (i == matrixRowSize - 1)
			j++;
		else
			i++;
		while (count) {
			k = (*returnSize) - count;
			a[k] = matrix[i][j];
			count--;
			if ((i == 0) || (j == matrixColSize - 1)) {
				break;
			} else {
				i--;
				j++;
			}
		}
		if (j == matrixColSize - 1)
			i++;
		else
			j++;
		while(count) {
			k = (*returnSize) - count;
			a[k] = matrix[i][j];
			count--;
			if ((i == matrixRowSize - 1) || (j == 0)) {
				break;
			} else {
				i++;
				j--;
			}
		}
	}
	return a;
}

int main(void)
{
	int *result = NULL;
	int result_len = -1;
	int **matrix = NULL;
	int row = -1;
	int col = -1;
	int i = 0;
	int j = 0;

	printf("\nEnter the row size of matrix:");
	scanf("%d", &row);
	if (row < 1) {
		printf("\nInvalid size of the row.\n");
		return -1;
	}

	printf("\nEnter the column size of matrix:");
	scanf("%d", &col);
	if (col < 1) {
		printf("\nInvalid size of the col.\n");
		return -1;
	}

	matrix = malloc(row * sizeof(int *));
	for (i = 0; i < row; i++) {
		matrix[i] = malloc(col * sizeof(int));
	}

	printf("\nEnter matrix values:\n");
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			printf("matrix[%d][%d]: ", i, j);
			scanf("%d", &(matrix[i][j]));
		}
	}

	result = findDiagonalOrder(matrix, row, col, &result_len);

	printf("\nResult: [\t");
	for (i = 0; i < result_len; i++) {
		printf("%d\t", result[i]);
	}
	printf("]\n");


	return 0;
}
