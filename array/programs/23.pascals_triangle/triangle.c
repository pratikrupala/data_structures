#include <stdio.h>
#include <stdlib.h>

int** generate(int numRows, int** columnSizes) {
	int i = 0, j = 0;
	int **result = NULL;

	if (numRows < 1) {
		printf("\nInvalid number of rows.\n");
		return NULL;
	}

	*columnSizes = malloc(numRows * sizeof(int));
	if (!(*columnSizes)) {
		printf("\nFailed to allocate memory for size array.\n");
		return NULL;
	}

	for (i = 0; i < numRows; i++) {
		*(*columnSizes + i) = i + 1;
	}

	result = malloc(numRows * sizeof (int *));
	if (!result) {
		printf("Failed to allocate memory for result.\n");
		return NULL;
	}

	for (i = 0;  i < numRows; i++) {
		result[i] = (int *)malloc(*(*columnSizes+i) * sizeof(int));
	}

	result[0][0] = 1;
	for (i = 1; i < numRows; i++) {
		for (j = 0; j < *(*columnSizes+i); j++) {
			if (j == 0) {
				result[i][j] = result[i - 1][j];
			} else if (j == (*(*columnSizes+i) - 1)) {
				result[i][j] = result[i - 1][j - 1];                     
			} else {
				result[i][j] = result[i - 1][j - 1] + result[i - 1][j];
			}
		}
	}

	for (i = 0; i < numRows; i++) {
		for (j = 0; j < *(*columnSizes+i); j++) {
			printf("%d\t", result[i][j]);
		}
		printf("\n");
	}

	return result;
}

int main(void)
{
	int **result = NULL;
	int *colsize = NULL; 
	int r = -1;
	int i = 0;
	int j = 0;

	printf("\nEnter the length of the Pascal's triangle: ");
	scanf("%d", &r);

	result = generate(r, &colsize);

	for (i = 0; i < r; i++) {
		for (j = 0; j < colsize[i]; j++) {
			printf("%d\t", result[i][j]);
		}
		printf("\n");
	}

	return 0;
}
