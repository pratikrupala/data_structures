#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generate_lps(char *pat, int l, int *lps)
{
	int i = 1;
	int j = 0;

	lps[0] = 0;
	while (i < l) {
		if (pat[i] == pat[j]) {
			lps[i] = ++j;
			i++;
		} else {
			if (j == 0) {
				lps[i] = 0;
				i++;
			} else {
				j = lps[j - 1];
			}
		}
	}
}

void search_pattern(char *text, int tlen, char *pat, int plen, int *lps)
{
	int k = 0;
	int m = 0;

	while (k < tlen) {
		if (text[k] == pat[m]) {
			k++;
			m++;
		}

		if (m == plen) {
			printf("\nFound pattern at index %d\n", k - m);
			m = lps[m - 1];
		} else if (k < tlen && text[k] != pat[m]) {
			if (m != 0) {
				m = lps[m - 1];
			} else {
				k++;
			}
		}
	}
}

int main(void)
{
	char *text = NULL;
	int tlen = -1;
	char *pat = NULL;
	int plen = -1;
	int *lps = NULL;
	int ret = -1;

	printf("Enter the length of the text string to be searched: ");
	scanf("%d", &tlen);
	if (tlen < 1) {
		fprintf(stderr, "Invalid text length.\n");
		goto out;
	}

	printf("Enter the length of the pattern for searching: ");
	scanf("%d", &plen);
	if (plen < 1) {
		fprintf(stderr, "Invalid pattern length.\n");
		goto out;
	}

	if (plen >= tlen) {
		fprintf(stderr, "Pattern length should be less than the"
				" text length.\n");
		goto out;
	}

	getchar();
	text = malloc((tlen + 1) * sizeof(char));
	if (!text) {
		fprintf(stderr, "Failed to allocate memory for text.\n");
		goto out;
	}
	printf("Enter the text string: ");
	fgets(text, tlen + 1, stdin);

	getchar();
	pat = malloc((plen + 1) * sizeof(char));
	if (!pat) {
		fprintf(stderr, "Failed to allocate memory for pattern.\n");
		goto out;
	}
	printf("Enter the pattern: ");
	fgets(pat, plen + 1, stdin);

	text[tlen] = '\0';
	pat[plen] = '\0';
	printf("DUMMY: text = %s, pattern = %s\n", text, pat);

	lps = malloc(plen * sizeof(int));
	if (!lps) {
		fprintf(stderr, "Failed to allocate memory for internal"
				" structure.\n");
		goto out;
	}

	generate_lps(pat, plen, lps);

	/*
	printf("LPS: \n");
	for (int i = 0; i < plen; i++) {
		printf("%d\t", lps[i]);
	}
	printf("\n");
	*/

	search_pattern(text, tlen, pat, plen, lps);

	ret = 0;
out:
	return ret;
}
