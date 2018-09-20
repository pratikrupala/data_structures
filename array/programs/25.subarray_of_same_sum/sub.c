#include <stdio.h>
#include <stdlib.h>

void find_subarray(int *p, int len, int sum)
{
	int start = 0;
	int end = -1;
	int current_sum = 0;

	for (int i = 0; i < len; i++) {
		current_sum += p[i];
		end++;
again:
		if (current_sum == sum) {
			printf("\nFound subarray: Between indices %d and %d\n", start, end);
			return;
		} else if (current_sum > sum) {
			while (current_sum > sum) {
				current_sum -= p[start];
				start++;
			}
			goto again;
		}
	}
	printf("\nNo subarray found\n");
}

int main(void)
{
	int *a = NULL;
	int len = -1;
	int num = -1;

	printf("Enter the length of the array: ");
	scanf("%d", &len);
	if (len < 1) {
		printf("\nInvalid length of the array.\n");
		return -1;
	}

	a = malloc(sizeof(int) * len);
	if (!a) {
		printf("\nFailed to allocate memory for the array.\n");
		return -1;
	}

	printf("Enter the numbers of the array:\n");
	for (int i = 0; i < len; i++) {
		printf("a[%d]: ", i);
		scanf("%d", &(a[i]));
	}

	printf("\n");
	for (int j = 0; j < len; j++) {
		printf("%d\t", a[j]);
	}
	printf("\n");

	printf("Enter the sum to be searched: ");
	scanf("%d", &num);

	find_subarray(a, len, num);

	return 0;
}
