#include <stdio.h>

int check(int a, int b)
{
	if ((a ^ b) >> 31)
		return 1;
	return 0;
}

int main(void)
{
	int a = -1;
	int b = -2;
	int ret = -1;

	ret = check(a, b);
	if (ret) {
		printf("Yes, numbers are opposite.\n");
	} else {
		printf("No, numbers are not opposite.\n");
	}

	return 0;
}
