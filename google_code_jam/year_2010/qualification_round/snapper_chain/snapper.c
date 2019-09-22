#include<stdio.h>

int is_bulb_on(int n, int k)
{
	int i = 0;
	int fail = 0;

	for (i = 0; i < n; i++) {
		if (!(k % 2)) {
			fail = 1;
			break;
		}
		k = k / 2;
	}

	if (fail)
		return 0;
	else
		return 1;
}

int main(void)
{
	int ret = -1;
	int tc = -1;
	int i = 0;

	scanf("%d", &tc);
	if ((tc < 1) || (tc > 10000)) {
		printf("Invalid number of test cases.\n");
		goto out;
	}

	for (i = 1; i <= tc; i++) {
		int n = 0;
		int k = 0;

		scanf("%d", &n);
		if ((n < 1) || (n > 30)) {
			printf("Invalid number of snapper devices.\n");
			goto out;
		}

		scanf("%d", &k);
		if ((k < 0) || (k > 100000000)) {
			printf("Invalid number of snaps.\n");
			goto out;
		}

		ret = is_bulb_on(n, k);
		if (ret == 1) {
			printf("Case #%d: ON\n", i);
		} else {
			printf("Case #%d: OFF\n", i);
		}
	}
	ret = 0;
out:
	return ret;
}
