#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 10

char **get_pattern_tokens(char *pattern, int len)
{
	int i = 0;
	char **tokens = NULL;
	char *current = pattern;

	tokens = malloc(len * sizeof(char *));
	if (!tokens) {
		printf("Failed to allocate memory for tokens.\n");
		goto out;
	}

	for (i = 0; i < len; i++) {
		tokens[i] = malloc(len * sizeof(char));
		if (!tokens[i]) {
			printf("Failed to allocate memory for token number"
					" %d.\n", i + 1);
			goto release_tokens;
		}
	}

	i = 0;
	while (*current != '\0' && len > 0) {
		if (*current == '(') {
			current++;
			tokens[i] = strtok(current, ")");
			current = current + (strlen(tokens[i]) + 1);
			i++;
		} else {
			if (!tokens[i])
				printf("No token i = %d.\n", i);
			*(tokens[i]) = *current;
			current++;
			tokens[i][1] = '\0';
			i++;
		}
		len--;
	}
	goto out;

release_tokens:
	free(tokens);
out:
	return tokens;
}

int get_output_count(char **words, int no_of_words, char *pattern, int len)
{
	char **tokens = NULL;
	int i = 0;
	int j = 0;
	int k = 0;
	int inner_count = 0;
	int found_it = 0;
	int count = 0;
	int ret = -1;

	tokens = get_pattern_tokens(pattern, len);
	if (!tokens) {
		printf("Failed to tokenize the pattern.\n");
		goto out;
	}
	/*
	for (i = 0; i < len; i++) {
		printf("Token#%d: %s\n", i + 1, tokens[i]);
	}
	*/

	for (i = 0; i < no_of_words; i++) {
		inner_count = 0;
		for (j = 0; j < len; j++) {
			found_it = 0;
			for (k = 0; k < strlen(tokens[j]); k++) {
				if (words[i][j] == tokens[j][k]) {
					found_it = 1;
					break;
				}
			}
			if (found_it) {
				inner_count++;
			} else {
				break;
			}
		}
		if (inner_count == len)
			count++;
	}

	ret = count;
out:
	return ret;
}

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

int main(void)
{
	int len = -1;
	int w = -1;
	int tc = -1;
	int i = 0;
	char **words = NULL;
	char *pattern = NULL;
	int *output = NULL;
	int ret = -1;

	scanf("%d", &len);
	if (len < 1 || len > 15) {
		printf("Invalid length of the words.\n");
		goto out;
	}
	scanf("%d", &w);
	if (w < 1 || w > 5000) {
		printf("Invalid number of the words.\n");
		goto out;
	}
	scanf("%d", &tc);
	if (tc < 1 || tc > 500) {
		printf("Invalid number of the test cases.\n");
		goto out;
	}

	words = malloc(w * sizeof(char *));
	if (!words) {
		printf("Failed to allocate memory for words. Aborting.\n");
		goto out;
	}
	for (i = 0; i < w; i++) {
		words[i] = malloc((len + 1) * sizeof(char));
		if (!words[i]) {
			printf("Failed to allocate memory for words."
					" Aborting.\n");
			goto release_word_pointer;
		}
	}

	for (i = 0; i < w; i++) {
		getchar();
		if (fgets(words[i], len + 1, stdin) == NULL) {
			printf("Failed to read words from stdin. Aborting.\n");
			goto release_words;
		}
	}

	/*
	printf("Printing words\n");
	for (i = 0; i < w; i++) {
		printf("word[%d]: %s\n", i + 1, words[i]);
	}
	*/

	output = malloc(tc * sizeof(int));
	if (!output) {
		printf("Failed to allocate memory for output array."
				" Aborting.\n");
		goto release_words;
	}
	getchar();
	memset(output, 0, sizeof(int) * tc);
	for (i = 0; i < tc; i++) {
		pattern = get_pattern();
		output[i] = get_output_count(words, w, pattern, len);
		free(pattern);
	}

	for (i = 0; i < tc; i++) {
		printf("Case #%d: %d\n", i + 1, output[i]);
	}

	ret = 0;
	free(output);
release_words:
	for (i = 0; i < w; i++) {
		free(words[i]);
	}
release_word_pointer:
	free(words);
out:
	return ret;
}
