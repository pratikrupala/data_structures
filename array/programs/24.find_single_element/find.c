#include <stdio.h>
#include <stdlib.h>

int find_num(int *p, int len)
{
	int a = 0;
	int b = 0;
	int a1 = 0;

	for (int i = 0; i < len; i++) {
		a1 = (a & ~p[i]) | (b & p[i]);
		b = (b & ~p[i]) | (~a & ~b & p[i]);
		a = a1;
	}

	return (a | b);
}

int main(void)
{
	int *a = NULL;
	int len = -1;
	int result = -1;

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

	result = find_num(a, len);
	printf("\nSingle num: %d\n", result);

	return 0;
}
