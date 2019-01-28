#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENG_NAME_SZ	100

void *bitmap_ptr = NULL;
int count = 0;

void set_bit(int n)
{
	int byte = n / 8;
	int bit = n % 8;
	char *ptr = bitmap_ptr;
	int *l = NULL;

	ptr += byte;
	l = (int *) ptr;

	*l = *l | (1 << bit);
}

void clear_bit(int n)
{
	int byte = n / 8;
	int bit = n % 8;
	char *ptr = bitmap_ptr;
	int *l = NULL;

	ptr += byte;
	l = (int *) ptr;

	*l = *l & ~(1 << bit);
}

int get_bit(int n)
{
	int byte = n / 8;
	int bit = n % 8;
	char *ptr = bitmap_ptr;
	int *l = NULL;

	ptr += byte;
	l = (int *) ptr;

	return ((*l >> bit) & 1);
}

int setup_bitmap(int size)
{
	int actual_size = 0;

	if ((size % 8) == 0) {
		actual_size = size / 8;
	} else {
		if ((size / 8) == 0)
			actual_size = 1;
		else
			actual_size = (size / 8) + 1;
	}

	bitmap_ptr = (void *) malloc(actual_size * sizeof(char));
	if (bitmap_ptr)
		return 0;
	else
		return 1;
}

char *match_string(char *engine, char *query)
{
	if (strcmp(engine, query) == 0)
		return query;
	else
		return NULL;
}

int find_query_str(char *query, char **engines, int n)
{
	int i = 0;
        int ret = -1;

	for (i = 0; i < n; i++) {
                if ((match_string(engines[i], query)) != NULL) {
			return i;
		}
	}

	return ret;
}

int check_query(char *query, char **engines, int n)
{
	int i = 0;
	int ret = -1;

	i = find_query_str(query, engines, n);
	if (i == -1) {
		goto out;
	}
	if (!get_bit(i)) {
		set_bit(i);
		count++;
		if (count == n) {
			ret = i;
			goto out;
		}
	}
out:
	return ret;
}

void clear_bitmap(int total_bits, int exception)
{
	int i = 0;

	for (i = 0; i < total_bits; i++) {
		if (i != exception)
			clear_bit(i);
	}
}

int main(void)
{
	int test_cases = 0;
	int total_engines = 0;
	int total_queries = 0;
	char *query_str = NULL;
	char **engines;
	int eng_switch = 0;
	int *switch_count = NULL;
	int i = 0;
	int j = 0;
	int ret = -1;

	scanf("%d", &test_cases);

	switch_count = malloc(test_cases * sizeof(int));

	for (i = 0; i < test_cases; i++) {
		count = 0;
		total_engines = 0;
		total_queries = 0;
		switch_count[i] = 0;
		eng_switch = -1;
		scanf("%d", &total_engines);
		engines = malloc(total_engines * sizeof(char *));
		if (setup_bitmap(total_engines)) {
			printf("Failed tp setup bitmap.\nAborting.\n");
			goto out;
		}
		getchar();
		for (j = 0; j < total_engines; j++) {
			engines[j] = malloc(ENG_NAME_SZ * sizeof(char));
			if (engines[j] == NULL) {
				printf("Failed to allocate memory for engine"
						" no %d. Aborting\n", j);
				goto out;
			}
		}
		for (j = 0; j < total_engines; j++) {
			if (fgets(engines[j], ENG_NAME_SZ, stdin) == NULL) {
				printf("The fgets() failed to get engine no"
						" %d.\n Aborting", j);
				goto out;
			}
			engines[j] = strtok(engines[j], "\n");
		}
		scanf("%d", &total_queries);
		getchar();
		for (j = 0; j < total_queries; j++) {
			query_str = malloc(ENG_NAME_SZ * sizeof(char));
			memset(query_str, '\0', ENG_NAME_SZ * sizeof(char));
			if (fgets(query_str, ENG_NAME_SZ, stdin) == NULL) {
				printf("The fgets() failed to get query no"
						" %d.\n Aborting.", j);
				goto out;
			}
			query_str = strtok(query_str, "\n");
			eng_switch = check_query(query_str, engines,
					total_engines);
			if (eng_switch != -1) {
				if ((eng_switch < 0) ||
					(eng_switch > total_engines - 1)) {
					printf("Failed to get the remaining"
							"engine no.\n"
							" Aborting.");
					goto out;
				}
				switch_count[i]++;
				clear_bitmap(total_engines, eng_switch);
				eng_switch = -1;
				count = 1;
			}
			if (query_str)
				free(query_str);
		}
		if (bitmap_ptr)
			free(bitmap_ptr);
		for (j = 0; j < total_engines; j++) {
			if (engines[j])
				free(engines[j]);
		}
	}

	for (i = 0; i < test_cases; i++) {
		printf("Case #%d: %d\n", i + 1, switch_count[i]);
	}
	ret = 0;
out:
	return ret;
}
