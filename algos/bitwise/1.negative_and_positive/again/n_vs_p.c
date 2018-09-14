#include <stdio.h>

int check(int a, int b)
{
	if ((a ^ b) < 0)
		return 1;
	return 0;
}

int main(void)
{
	int a = -1;
	int b = 2;
	int ret = -1;

	ret = check(a, b);
	if (ret == 1) {
		printf("Yes, numbers are opposite.\n");
	} else {
		printf("No, numbers are not opposite.\n");
	}

	return 0;
}
