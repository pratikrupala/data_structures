#include <stdio.h>
#include <stdlib.h>

void my_merge(int *arr, int left, int m, int right)
{
	int i = 0;
	int j = 0;
	int p1 = 0;
	int p2 = 0;
	int l1 = m - left + 1;
	int l2 = right - m;
	int first[l1];
	int second[l2];

	for (i = 0; i < l1; i++) {
		first[i] = arr[left + i];
	}

	for (i = 0; i < l2; i++) {
		second[i] = arr[m + i + 1];
	}

	j = left;
	while ((p1 < l1) && (p2 < l2)) {
		if (first[p1] > second[p2]) {
			arr[j] = second[p2];
			p2++;
		} else {
			arr[j] = first[p1];
			p1++;
		}
		j++;
	}

	while (p1 < l1) {
		arr[j] = first[p1++];
		j++;
	}

	while (p2 < l2) {
		arr[j] = second[p2++];
		j++;
	}
}

void my_merge_sort(int *arr, int left, int right)
{
	int m = 0;
	if (left < right) {
		m = left + (right - left) / 2;
		my_merge_sort(arr, left, m);
		my_merge_sort(arr, m + 1, right);
		my_merge(arr, left, m, right);
	}
}

int main(void)
{
	int i = 0;
	int ret = -1;
	int *arr = NULL;
	int len = 0;

	printf("Enter the length of the array: ");
	scanf("%d", &len);
	if (len < 1) {
		fprintf(stderr, "Invalid number of elements. Aborting.");
		goto out;
	}

	arr = malloc(len * sizeof(int));
	if (!arr) {
		fprintf(stderr, "Failed to allocate memory for array."
				" Aborting.");
		goto out;
	}

	printf("Enter your array elements: \n");
	for (i = 0; i < len; i++) {
		scanf("%d", &arr[i]);
	}
	my_merge_sort(arr, 0, len - 1);
	printf("Result: \n");
	for (i = 0; i < len; i++) {
		printf("%d\t", arr[i]);
	}
	printf("\n");
	ret = 0;
out:
	return ret;
}
