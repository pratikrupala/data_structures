#include <stdio.h>
#include <stdlib.h>

int *
plus_one(int* digits, int digitsSize, int* returnSize)
{
    int i = 0;
    int flag = 0;
    int carry = 1;
    int *result = NULL;
			        
    for (i = 0; i < digitsSize; i++) {
        if (digits[i] != 9) {
            flag = 1;
            break;
        }
    }
				    
    if (!flag) {
        *returnSize = digitsSize + 1;
    } else {
        *returnSize = digitsSize;
    }
				        
    result = malloc((*returnSize) * sizeof(int));
			    
    for (i = digitsSize - 1; i >= 0; i--) {
         if (carry) {
            result[i] = (digits[i] + carry) % 10;
            carry = (digits[i] + carry) / 10;
         } else {
            result[i] = digits[i];
         }
    }
					        
    if (*returnSize > digitsSize) {
        result[0] = carry;
    }
						    
    return result;
}

int
main(void)
{
	int *a = NULL;
        int *result = NULL;
	int len;
	int result_len;
	int i = 0;

	printf("Enter the length of the array: ");
	scanf("%d", &len);

	a = malloc(len * sizeof(int));
	if (!a) {
		printf("Failed to allocate memory for an array.\n");
		return -1;
	}

	printf("Enter array elements: \n");
	for (i = 0; i < len; i++) {
		printf("a[%d]: ", i);
		scanf("%d", &(a[i]));
	}

	result = plus_one(a, len, &result_len);

	printf("\nResult array:\n");
	for (i = 0; i < result_len; i++) {
		printf("a[%d]: %d\t", i, result[i]);
	}
	printf("\n");

	return 0;
}
