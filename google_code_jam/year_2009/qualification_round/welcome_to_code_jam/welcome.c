#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 10
#define MSG "welcome to code jam"

char *get_pattern(void)
{
	int size = 0;
	int offset = 0;
	char *dummy = NULL;
	char *pattern = NULL;
	char *patt_buf = NULL;

	patt_buf = (char *) malloc((BUF_SIZE + 1) * sizeof(char));
	if (!patt_buf) {
		printf("Failed to allocate memory for pattern buffer."
				" Aborting.\n");
		goto out;
	}
	while(1) {
		memset(patt_buf, '\0', BUF_SIZE);
		if (fgets(patt_buf, BUF_SIZE, stdin) == NULL) {
			if (strlen(patt_buf) > 0) {
				size = 0;
				if (pattern)
					size = strlen(pattern);
				size += strlen(patt_buf) + 1;
				pattern = realloc(pattern, size);
				memcpy(pattern + offset, patt_buf,
						strlen(patt_buf));
				dummy = pattern + offset + strlen(patt_buf);
				*dummy = '\n';
			}
			break;
		} else {
			size = 0;
			if (pattern) {
				size = strlen(pattern);
				size += strlen(patt_buf);
			} else {
				size += strlen(patt_buf) + 1;
			}
			pattern = realloc(pattern, size);
			memcpy(pattern + offset, patt_buf, strlen(patt_buf));
			offset += strlen(patt_buf);
			dummy = pattern + offset;
			if (strlen(patt_buf) < (BUF_SIZE - 1)) {
				*dummy = '\0';
				break;
			} else if (strlen(patt_buf) == (BUF_SIZE - 1) &&
					(patt_buf[BUF_SIZE - 2] == '\n')) {
				*dummy = '\0';
				break;
			} else
				*dummy = '\n';
		}
	}
//	if (pattern)
//		printf("Pattern: %s", pattern);
out:
	return pattern;
}

int count_messages(char *message)
{
	int len = strlen(message);
	len--;
	int result[len][19];
	int i = 0;
	int j = 0;

	for (i = 0; i < len; i++) {
		for (j = 0; j < 19; j++) {
			if (message[i] == MSG[j]) {
				if (i == 0 && j == 0) {
					result[i][j] = 1;
				} else if (i == 0) {
					result[i][j] = result[i][j - 1];
				} else if (j == 0) {
					result[i][j] = result[i - 1][j] + 1;
				} else {
					result[i][j] = result[i- 1][j] + result[i][j - 1];
				}
			} else {
				if (i != 0) {
					result[i][j] = result[i - 1][j];
				} else {
					result[i][j] = 0;
				}
			}
			result[i][j] %= 10000;
		}
	}

	return result[len - 1][18];
}

int main(void)
{
	int ret = -1;
	int tc = -1;
	char *pattern = NULL;
	int i = 0;

	scanf("%d", &tc);
	if (tc < 1 || tc > 100) {
		printf("Invalid number of test cases.\n");
		goto out;
	}

	getchar();
	for (i = 0; i < tc; i++) {
		int result = 0;
		pattern = get_pattern();
		result = count_messages(pattern);
		printf("Case #%d: %04d\n", i + 1, result);
		free(pattern);
	}
	ret = 0;
out:
	return ret;
}
