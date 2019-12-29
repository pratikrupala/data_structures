#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl_support.h"

struct lnode {
        char second;
        char result;
        struct lnode * next;
};

struct lnode *Q_head = NULL;
struct lnode *Q_last = NULL;
struct lnode *W_head = NULL;
struct lnode *W_last = NULL;
struct lnode *E_head = NULL;
struct lnode *E_last = NULL;
struct lnode *R_head = NULL;
struct lnode *R_last = NULL;
struct lnode *A_head = NULL;
struct lnode *A_last = NULL;
struct lnode *S_head = NULL;
struct lnode *S_last = NULL;
struct lnode *D_head = NULL;
struct lnode *D_last = NULL;
struct lnode *F_head = NULL;
struct lnode *F_last = NULL;

struct lnode *Q_o_head = NULL;
struct lnode *Q_o_last = NULL;
struct lnode *W_o_head = NULL;
struct lnode *W_o_last = NULL;
struct lnode *E_o_head = NULL;
struct lnode *E_o_last = NULL;
struct lnode *R_o_head = NULL;
struct lnode *R_o_last = NULL;
struct lnode *A_o_head = NULL;
struct lnode *A_o_last = NULL;
struct lnode *S_o_head = NULL;
struct lnode *S_o_last = NULL;
struct lnode *D_o_head = NULL;
struct lnode *D_o_last = NULL;
struct lnode *F_o_head = NULL;
struct lnode *F_o_last = NULL;

int Q_o_len = 0;
int W_o_len = 0;
int E_o_len = 0;
int R_o_len = 0;
int A_o_len = 0;
int S_o_len = 0;
int D_o_len = 0;
int F_o_len = 0;

void clean_up(void)
{
	Q_head = NULL;
	Q_last = NULL;
	W_head = NULL;
	W_last = NULL;
	E_head = NULL;
	E_last = NULL;
	R_head = NULL;
	R_last = NULL;
	A_head = NULL;
	A_last = NULL;
	S_head = NULL;
	S_last = NULL;
	D_head = NULL;
	D_last = NULL;
	F_head = NULL;
	F_last = NULL;

	Q_o_head = NULL;
	Q_o_last = NULL;
	W_o_head = NULL;
	W_o_last = NULL;
	E_o_head = NULL;
	E_o_last = NULL;
	R_o_head = NULL;
	R_o_last = NULL;
	A_o_head = NULL;
	A_o_last = NULL;
	S_o_head = NULL;
	S_o_last = NULL;
	D_o_head = NULL;
	D_o_last = NULL;
	F_o_head = NULL;
	F_o_last = NULL;

	Q_o_len = 0;
	W_o_len = 0;
	E_o_len = 0;
	R_o_len = 0;
	A_o_len = 0;
	S_o_len = 0;
	D_o_len = 0;
	F_o_len = 0;
}

int c_list_add(struct lnode **head, struct lnode **last, char b, char c)
{
	int res = -1;
	struct lnode *new_node = malloc(sizeof(struct lnode));
	if (!new_node) {
		printf("\nFailed to allocate memory for %c.\n", b);
		goto out;
	}
	new_node->second = b;
	new_node->result = c;
	new_node->next = NULL;

	if (*head) {
		(*last)->next = new_node;
		 *last = (*last)->next;
	} else {
		*head = new_node;
		*last = new_node;
	}
	res = 0;
//	free(new_node);

out:
	return res;
}

void iter_c_list(struct lnode *head) {
	struct lnode *iter = head;
	if (head)
		printf("\n");
	while (iter) {
		printf("%c\t", iter->second);
		printf("%c\t", iter->result);
		iter = iter->next;
		if (iter)
			printf("\n");
	}
	if (head)
		printf("\n");
}

int add_c_element(char *element)
{
	int ret = -1;
	for (int i = 0; i < 2; i++) {
		switch (element[i]) {
		case 'Q':
			ret = c_list_add(&Q_head, &Q_last, element[i?0:1], element[2]);
			break;
		case 'W':
			ret = c_list_add(&W_head, &W_last, element[i?0:1], element[2]);
			break;
		case 'E':
			ret = c_list_add(&E_head, &E_last, element[i?0:1], element[2]);
			break;
		case 'R':
			ret = c_list_add(&R_head, &R_last, element[i?0:1], element[2]);
			break;
		case 'A':
			ret = c_list_add(&A_head, &A_last, element[i?0:1], element[2]);
			break;
		case 'S':
			ret = c_list_add(&S_head, &S_last, element[i?0:1], element[2]);
			break;
		case 'D':
			ret = c_list_add(&D_head, &D_last, element[i?0:1], element[2]);
			break;
		case 'F':
			ret = c_list_add(&F_head, &F_last, element[i?0:1], element[2]);
			break;
		default:
			break;
		}
	}

	return ret;
}

int parse_c_list(char **c_list, int len)
{
	int i = 0;
	int ret = -1;

	for (i = 0; i < len; i++) {
		ret = add_c_element(c_list[i]);
		if (ret)
			break;
	}

	return ret;
}

void iter_o_list(struct lnode *head) {
	struct lnode *iter = head;
	if (head)
		printf("\n");
	while (iter) {
		printf("%c\t", iter->second);
		iter = iter->next;
		if (iter)
			printf("\n");
	}
	if (head)
		printf("\n");
}

int o_list_add(struct lnode **head, struct lnode **last, char c)
{
	int res = -1;
	struct lnode *new_node = malloc(sizeof(struct lnode));
	if (!new_node) {
		printf("\nFailed to allocate memory for %c.\n", c);
		goto out;
	}
	new_node->second = c;
	new_node->next = NULL;

	if (*head) {
		(*last)->next = new_node;
		 *last = (*last)->next;
	} else {
		*head = new_node;
		*last = new_node;
	}
	res = 0;
//	free(new_node);

out:
	return res;
}

int add_o_element(char *element)
{
	int ret = -1;
	for (int i = 0; i < 2; i++) {
		switch (element[i]) {
		case 'Q':
			ret = o_list_add(&Q_o_head, &Q_o_last, element[i?0:1]);
			Q_o_len++;
			break;
		case 'W':
			ret = o_list_add(&W_o_head, &W_o_last, element[i?0:1]);
			W_o_len++;
			break;
		case 'E':
			ret = o_list_add(&E_o_head, &E_o_last, element[i?0:1]);
			E_o_len++;
			break;
		case 'R':
			ret = o_list_add(&R_o_head, &R_o_last, element[i?0:1]);
			R_o_len++;
			break;
		case 'A':
			ret = o_list_add(&A_o_head, &A_o_last, element[i?0:1]);
			A_o_len++;
			break;
		case 'S':
			ret = o_list_add(&S_o_head, &S_o_last, element[i?0:1]);
			S_o_len++;
			break;
		case 'D':
			ret = o_list_add(&D_o_head, &D_o_last, element[i?0:1]);
			D_o_len++;
			break;
		case 'F':
			ret = o_list_add(&F_o_head, &F_o_last, element[i?0:1]);
			F_o_len++;
			break;
		default:
			break;
		}
	}

	return ret;
}

int parse_o_list(char **o_list, int len)
{
	int i = 0;

//	for (i = 0; i < len; i = i + 2) {
	for (i = 0; i < len; i++) {
		add_o_element(o_list[i]);
	}

	return 0;
}

int check_char(char element)
{
	int ret = 0;
	switch (element) {
		case 'Q':
		case 'W':
		case 'E':
		case 'R':
		case 'A':
		case 'S':
		case 'D':
		case 'F':
			ret = 1;
			break;
		default:
			break;
	}

	return ret;
}

int search_c_list(struct lnode *head, char b, char *c)
{
	int test = 0;
	struct lnode *iter = head;
	while (iter) {
		if (iter->second == b) {
			*c = iter->result;
			test = 1;
			break;
		}
		iter = iter->next;
	}
	return test;
}

int check_c_list(char a, char b, char *c)
{
	int ret = 0;

	switch (a) {
		case 'Q':
			ret = search_c_list(Q_head, b, c);
			break;
		case 'W':
			ret = search_c_list(W_head, b, c);
			break;
		case 'E':
			ret = search_c_list(E_head, b, c);
			break;
		case 'R':
			ret = search_c_list(R_head, b, c);
			break;
		case 'A':
			ret = search_c_list(A_head, b, c);
			break;
		case 'S':
			ret = search_c_list(S_head, b, c);
			break;
		case 'D':
			ret = search_c_list(D_head, b, c);
			break;
		case 'F':
			ret = search_c_list(F_head, b, c);
			break;
		default:
			break;
	}

	return ret;
}

int search_o_list(struct lnode *head, int len, char **c)
{
	int m = 0;
	int test = 0;
	struct lnode *iter = head;

	if (iter) {
		test = 1;
		*c = malloc(len * sizeof(char));
	}
	while (iter) {
		(*c)[m] = iter->second;
		iter = iter->next;
		m++;
	}
	return test;
}

int check_o_list(char a, char **c, int *len)
{
	int ret = 0;

	switch (a) {
		case 'Q':
			ret = search_o_list(Q_o_head, Q_o_len, c);
			*len = Q_o_len;
			break;
		case 'W':
			ret = search_o_list(W_o_head, W_o_len, c);
			*len = W_o_len;
			break;
		case 'E':
			ret = search_o_list(E_o_head, E_o_len, c);
			*len = E_o_len;
			break;
		case 'R':
			ret = search_o_list(R_o_head, R_o_len, c);
			*len = R_o_len;
			break;
		case 'A':
			ret = search_o_list(A_o_head, A_o_len, c);
			*len = A_o_len;
			break;
		case 'S':
			ret = search_o_list(S_o_head, S_o_len, c);
			*len = S_o_len;
			break;
		case 'D':
			ret = search_o_list(D_o_head, D_o_len, c);
			*len = D_o_len;
			break;
		case 'F':
			ret = search_o_list(F_o_head, F_o_len, c);
			*len = F_o_len;
			break;
		default:
			break;
	}

	return ret;
}

int search_opposites(struct avl_node *root, char **o_list, int len)
{
	struct avl_node *node = NULL;
	int ret = 0;

	for (int i = 0; i < len; i++) {
		node = (struct avl_node *) modified_avl_search(root,
				(*o_list)[i]);
		if (node) {
			ret = 1;
			break;
		}
	}

	return ret;
}

int process_list(char *list, int llen, char **result, int *k)
{
	struct avl_node *root = NULL;
	*result = malloc(llen * sizeof(char));
	char *o_list = NULL;
	int unbalanced = 0;
	int combination_happened = 0;
	char previous;
	int start = 1;
	char c;
	int ret = -1;
	int test = -1;
	*result[0] = list[0];
	for (int i = 0, j = -1; i < llen; i++, j++) {
		int len = 0;
		unbalanced = 0;
		o_list = NULL;
		if (start && check_char(list[i])) {
			if (check_o_list(list[i], &o_list, &len))
				insert_char(&root, list[i], &unbalanced);
			(*result)[(*k)] = list[i];
			(*k)++;
			start = 0;
			continue;
		}
		if (check_char(list[i])) {
			if (combination_happened) {
				previous = (*result)[(*k) - 1];
				combination_happened = 0;
			} else {
				previous = list[j];
			}
			test = check_c_list(list[i], previous, &c);
			if (test) {
				(*k)--;
				(*result)[(*k)] = c;
				(*k)++;
				combination_happened = 1;
//				unbalanced = 0;
//				delete_char(&root, list[i], &unbalanced);
				unbalanced = 0;
				delete_char(&root, previous, &unbalanced);
			} else {
				test = check_o_list(list[i], &o_list, &len);
				if (test) {
					ret = search_opposites(root,
							&o_list, len);
					if (ret) {
						*k = 0;
						free(*result);
						*result = malloc(llen *
								sizeof(char));
						root = NULL;
						start = 1;
					} else {
						(*result)[(*k)] = list[i];
	                                        (*k)++;
						insert_char(&root, list[i],
								&unbalanced);
					}
				} else {
					(*result)[(*k)] = list[i];
					(*k)++;
				}
			}
		} else {
			(*result)[(*k)] = list[i];
			(*k)++;
		}
	}
	ret = 0;
	return ret;
}

int main(void)
{
	int ret = -1;
	int tc = -1;

	scanf("%d", &tc);
	if ((tc < 1) | (tc > 100)) {
		printf("Invalid number of test cases.\n");
		goto out;
	}

	for (int i = 1; i <= tc; i++) {
		int k = 0;
		int c_no = -1;
		int o_no = -1;
		int list_len = -1;
		char **c_list = NULL;
		char **o_list = NULL;
		char *list = NULL;
		char *result = NULL;

		scanf("%d", &c_no);
		if ((c_no < 0) || (c_no > 36 )) {
			printf("Invalid number of combination numbers.\n");
			goto out;
		}

		if (!c_no)
			goto skip_c_list;
		c_list = malloc(sizeof(char *) * c_no);
		if (!c_list) {
			printf("Failed to allocate memory for c_list"
				" pointers.\n");
			goto out;
		}

		for (int j = 0; j < c_no; j++) {
			c_list[j] = malloc(4 * sizeof(char));
			if (!c_list[j]) {
				printf("Failed to allocate memory for"
					" c_list[%d].\n", j + 1);
				goto out;
			}
		}

		for (int j = 0; j < c_no; j++) {
			getchar();
			fgets(c_list[j], 4, stdin);
		}

		ret = parse_c_list(c_list, c_no);
		if (ret) {
			printf("Failed to parse the combination list.\n");
			goto out;
		}
skip_c_list:
		scanf("%d", &o_no);
		if ((o_no < 0) || (o_no > 28 )) {
			printf("Invalid number of opposite numbers.\n");
			goto out;
		}

		if (!o_no)
			goto skip_o_list;
		o_list = malloc(sizeof(char *) * o_no);
		if (!o_list) {
			printf("Failed to allocate memory for o_list"
				" pointers.\n");
			goto out;
		}

		for (int j = 0; j < o_no; j++) {
			o_list[j] = malloc(3 * sizeof(char));
			if (!o_list[j]) {
				printf("Failed to allocate memory for"
					" o_list[%d].\n", j + 1);
				goto out;
			}
		}

		for (int j = 0; j < o_no; j++) {
			getchar();
			fgets(o_list[j], 3, stdin);
		}

		ret = parse_o_list(o_list, o_no);
		if (ret) {
			printf("Failed to parse opposite list.\n");
			goto out;
		}
skip_o_list:
		scanf("%d", &list_len);
		if ((list_len < 1) || (list_len > 100)) {
			printf("Invalid number of list length.\n");
			goto out;
		}

		list = malloc((list_len + 1) * sizeof(char));
		if (!list) {
			printf("Failed to allocate memory for list.\n");
			goto out;
		}

		getchar();
		fgets(list, list_len + 1, stdin);

		//printf("List: %s\n", list);

		ret = process_list(list, list_len, &result, &k);
		if (ret)
			goto out;
		printf("Case #%d: [", i);
		for (int w = 0; w < k; w++) {
			printf("%c", result[w]);
			if (w != k - 1)
				printf(", ");
		}
		printf("]\n");
		clean_up();
		for (int j = 0; j < c_no; j++) {
			free(c_list[j]);
		}
		if (c_list)
			free(c_list);
		for (int j = 0; j < o_no; j++) {
			free(o_list[j]);
		}
		if (o_list)
			free(o_list);
		if (list)
			free(list);
//		free(result);
		/*
		for (int j = 0; j < c_no; j++) {
			printf("c[%d]: %s\n", j, c_list[j]);
		}
		for (int j = 0; j < o_no; j++) {
			printf("o[%d]: %s\n", j, o_list[j]);
		}
		iter_c_list(Q_head);
		iter_c_list(W_head);
		iter_c_list(E_head);
		iter_c_list(R_head);
		iter_c_list(A_head);
		iter_c_list(S_head);
		iter_c_list(D_head);
		iter_c_list(F_head);

		iter_o_list(Q_o_head);
		iter_o_list(W_o_head);
		iter_o_list(E_o_head);
		iter_o_list(R_o_head);
		iter_o_list(A_o_head);
		iter_o_list(S_o_head);
		iter_o_list(D_o_head);
		iter_o_list(F_o_head);
		*/
	}
	ret = 0;
out:
	return ret;
}
