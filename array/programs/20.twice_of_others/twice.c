#include <stdio.h>
#include <stdlib.h>

int find_index(int* nums, int numsSize) {
    int max = nums[0];
    int index = 0;
    int i = 0;
			    
    for (i = 1; i < numsSize; i++) {
        if (max < nums[i]) {
            max = nums[i];
            index = i;
        }
    }
			        
    for (i = 0; i < numsSize; i++) {
	if (i != index) {
	        if ((max/2) < nums[i])
        	    return -1;
	}
    }
					        
    return index;
}

int
main(void)
{
	int *a = NULL;
	int len = -1;
	int result = -1;
	int i = 0;

	printf("\nEnter the length of the array:\n");
	scanf("%d", &len);

	a = malloc(sizeof(int) * len);
	if (!a) {
		printf("\nFailed to allocate memory for an array.\n");
		return -1;
	}

	for (i = 0; i < len; i++) {
		printf("a[%d]: ", i);
		scanf("%d", &(a[i]));
	}

	result = find_index(a, len);
	printf("\nResult: %d\n", result);

	return 0;
}
