#include <stdio.h>
#include <stdlib.h>

int interpolation_search(int *arr, int n, int len)
{
	int loc = -1;
	int low = 0;
	int high = len - 1;

	while ((low <= high) && (arr[low] <= n) && (arr[high] >= n)) {
		loc = low + (((n - arr[low]) * (high - low))
				/(arr[high] - arr[low]));
		if (arr[loc] == n)
			return loc;
		if (arr[loc] > n)
			high = loc - 1;
		else
			low = loc + 1;
	}

	return -1;
}

int main(void)
{
	int i = 0;
	int *arr = NULL;
	int len = -1;
	int ret = -1;
	int n = -1;

	printf("\nEnter the length of the array: ");
	scanf("%d", &len);
	if (len < 1) {
		printf("\nInvalid length.\n");
		return -1;
	}

	arr = malloc(len * sizeof(int));
	if (!arr) {
		printf("\nFailed to allocate memory for array.\n");
		return -1;
	}

	printf("\nEnter the array elements:\n");
	for (i = 0; i < len; i++) {
		printf("%d: ", i);
		scanf("%d", &arr[i]);
	}

	printf("\nYour entered array:\n");
	for (i = 0; i < len; i++) {
		printf("%d\t", arr[i]);
	}
	printf("\n");

	printf("\nEnter the number to be searched in the array: ");
	scanf("%d", &n);

	ret = interpolation_search(arr, n, len);
	if (ret == -1) {
		printf("\nNo such number found in the array.\n");
	} else {
		printf("\nNumber %d found at index: %d\n", n, ret);
	}

	return 0;
}
