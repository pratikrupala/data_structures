#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char current = 'a';

struct node {
	int data;
	int visited;
	char color;
};

char find_stream(int i, int j, struct node **input, int row, int col)
{
	int orig = input[i][j].data;
	int least = input[i][j].data;
	int lrow = 0;
	int lcol = 0;
	int found = 0;

	if (input[i][j].visited) {
		return input[i][j].color;
	}

	if ((i < (row - 1)) && ((orig > input[i + 1][j].data) &&
				(least >= input[i + 1][j].data))) {
		least = input[i + 1][j].data;
		lrow = i + 1;
		lcol = j;
		found = 1;
	}
	if ((j < (col - 1)) && ((orig > input[i][j + 1].data) &&
				(least >= input[i][j + 1].data))) {
		least = input[i][j + 1].data;
		lrow = i;
		lcol = j + 1;
		found = 1;
	}
	if ((j > 0) && ((orig > input[i][j - 1].data) &&
				(least >= input[i][j - 1].data))) {
		least = input[i][j - 1].data;
		lrow = i;
		lcol = j - 1;
		found = 1;
	}
	if ((i > 0) && ((orig > input[i - 1][j].data) &&
				(least >= input[i - 1][j].data))) {
		least = input[i - 1][j].data;
		lrow = i - 1;
		lcol = j;
		found = 1;
	}

	if (found) {
		input[i][j].color = find_stream(lrow, lcol, input, row, col);
		input[i][j].visited = 1;
		return input[i][j].color;
	} else {
		input[i][j].color = current;
		input[i][j].visited = 1;
		return current++;
	}
}

int generate_watersheds(struct node **input, int row, int col)
{
	int i = 0;
	int j = 0;

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (!input[i][j].visited) {
				input[i][j].color = find_stream(i, j, input,
						row, col);
			}
		}
	}
	return 0;
}

int main(void)
{
	int k = 0;
	int ret = -1;
	int tc = -1;

	scanf("%d", &tc);
	if ((tc < 1) || (tc > 100)) {
		printf("Invalid number of testcases.\n");
		goto out;
	}

	for (k = 0; k < tc; k++) {
		int r = -1;
		int c = -1;
		int i = 0;
		int j = 0;

		scanf("%d", &r);
		if ((r < 1) || (r > 100)) {
			printf("Invalid number of rows.\n");
			goto out;
		}

		scanf("%d", &c);
		if ((c < 1) || (c > 100)) {
			printf("Invalid number of columns.\n");
			goto out;
		}

		struct node **input;

		input = malloc(r * sizeof(struct node *));
		if (!input) {
			printf("Failed to allocate memory for input.\n");
			goto out;
		}

		for (i = 0; i < r; i++) {
			input[i] = malloc(c * sizeof(struct node));
			if (!input[i]) {
				printf("Failed to allocate memory for "
						"input[%d].\n", i);
				goto out;
			}
		}

		for (i = 0; i < r; i++) {
			for (j = 0; j < c; j++) {
				scanf("%d", &(input[i][j].data));
				input[i][j].visited = 0;
			}
		}

		ret = generate_watersheds(input, r, c);
		if (ret)
			goto out;
		printf("Case #%d:\n", k + 1);
		for (i = 0; i < r; i++) {
			for (j = 0; j < c; j++) {
				printf("%c ", input[i][j].color);
			}
			printf("\n");
		}
		current = 'a';
	}

out:
	return ret;
}
