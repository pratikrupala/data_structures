#include <stdio.h>

struct input {
	int d;
	int v;
};

unsigned long get_money(struct input in[], int r, int k, int n)
{
	int flag = 0;
	unsigned long sum = 0;
	unsigned long result = 0;
	int marker = 0;
	int actual[n];
	unsigned long total = 0;
	int slab[n];
	unsigned long slab_total = 0;
	int count_down = r;
	int m = 0;
	int y = 0;
	int z = 0;

	for (m = 0; m < n; m++) {
		sum += in[m].d;
	}
	m = 0;
	while (1) {
		int tmp = 0;
		unsigned long check = sum;

		if (!flag && in[m].v) {
			flag = 1;
			marker = m;
		} else if (flag && (marker == m))
			break;
		in[m].v = 1;
		while ((tmp < k) && (tmp < check)) {
			tmp += in[m].d;
			if (n > 1) {
				m = ((m + 1) % n);
			} else {
				m = 0;
			}
		}
		if (m < 0)
			m = n - 1;
		if ((n > 1) && (tmp > k)) {
			if (m == 0)
				m = n - 1;
			else
				m -= 1;
			tmp -= in[m].d;
		}
		if (!flag) {
			slab[y++] = tmp;
			slab_total += tmp;
		} else {
			actual[z++] = tmp;
			total += tmp;
		}
	}

	int q = 0;
	if (r <= y) {
		if (r == y) {
			result = slab_total;
		} else {
			for (q = 0; (q < r); q++) {
				result += slab[q];
			}
		} 
	} else {
		int l = 0;
		int u = 0;

		result = slab_total;
		count_down -= y;
		l = (count_down / z);
		result += l * total;

		for (u = 0; u < (count_down % z); u++) {
			result += actual[u];
		}
	}	

	return result;
}

int main(void)
{
	int ret = -1;
	int tc = -1;
	int i = 0;
	unsigned long money = 0;

	scanf("%d", &tc);
	if ((tc < 1) || (tc > 50)) {
		printf("Invalid number of test cases.\n");
		goto out;
	}

	for (i = 1; i <= tc; i++) {
		int j = 0;
		int r = 0;
		int k = 0;
		int n = 0;

		scanf("%d", &r);
		if ((r < 1) || (r > 100000000)) {
			printf("Invalid number of R.\n");
			goto out;
		}

		scanf("%d", &k);
		if ((k < 1) || (k > 1000000000)) {
			printf("Invalid number of k.\n");
			goto out;
		}

		scanf("%d", &n);
		if ((n < 1) || (n > 1000)) {
			printf("Invalid number of N.\n");
			goto out;
		}

		struct input in[n];

		for (j = 0; j < n; j++) {
			scanf("%d", &in[j].d);
			in[j].v = 0;
		}
		money = get_money(in, r, k, n);
		printf("Case #%d: %lu\n", i, money);
	}
out:
	return ret;
}
