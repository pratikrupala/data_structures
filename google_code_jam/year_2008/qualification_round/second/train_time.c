#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl_support.h"

int main(void)
{
	int i = 0;
	int ret = -1;
	int tc = -1;
	int turn_around_time = -1;
	int trips_from_a = -1;
	int trips_from_b = -1;
	struct avl_node *a_start_time = NULL;
	int unbalanced_a_start = 0;
	struct avl_node *a_end_time = NULL;
	int unbalanced_a_end = 0;
	struct avl_node *b_start_time = NULL;
	int unbalanced_b_start = 0;
	struct avl_node *b_end_time = NULL;
	int unbalanced_b_end = 0;
	int count_a = 0;
	int count_b = 0;

	scanf("%d", &tc);
	if ((tc < 1) || (tc > 100)) {
		printf("Invalid number of test cases. Aborting.\n");
		goto out;
	}

	for (i = 0; i < tc; i++) {
		int k = 0;
		struct train_start_time *tst = NULL;
		struct train_end_time *tet = NULL;
		char *time_input = NULL;
		char *tok = NULL;
		a_start_time = NULL;
		a_end_time = NULL;
		b_start_time = NULL;
		b_end_time = NULL;
		scanf("%d", &turn_around_time);
		if ((turn_around_time < 0) || (turn_around_time > 60)) {
			printf("Invalid turn around time. It should be between"
					"0 and 5. Aborting.\n");
			goto out;
		}

		scanf("%d", &trips_from_a);
		if ((trips_from_a < 0) || (trips_from_a > 100)) {
			printf("Invalid number of trips from station A. "
					"Aborting.\n");
			goto out;
		}

		scanf("%d", &trips_from_b);
		if ((trips_from_b < 0) || (trips_from_b > 100)) {
			printf("Invalid number of trips from station B. "
					"Aborting.\n");
			goto out;
		}

		getchar();
		// Trips from station A to B
		for (k = 0; k < trips_from_a; k++) {
			int str_len = 0;
			unbalanced_a_start = 0;
			unbalanced_a_end = 0;
			time_input = malloc(MAX_SIZE * sizeof(char));
			tst = malloc(sizeof(struct train_start_time));
			tet = malloc(sizeof(struct train_end_time));
			memset(time_input, '\0', MAX_SIZE);
			if (!time_input) {
				printf("Failed to allocate memory for input "
						"time. Aborting.\n");
			}
			fgets(time_input, MAX_SIZE, stdin);
			str_len = strlen(time_input);
			time_input[str_len] = '\0';

			tok = strtok(time_input, ":");
			tst->start_hour = atoi(tok);
			tok = strtok(NULL, " :");
			tst->start_min = atoi(tok);
			tok = strtok(NULL, " :");
			tet->end_hour = atoi(tok);
			tok = strtok(NULL, "");
			tet->end_min = atoi(tok);

			if (tt_insert_node(&a_start_time, tst, tet, 1,
						&unbalanced_a_start)) {
				printf("Failed to add node for schedule"
						" number %d from station"
						" A. Aborting.\n", k);
				goto out;
			}
			if (tt_insert_node(&a_end_time, tst, tet, 0,
						&unbalanced_a_end)) {
				printf("Failed to add node for schedule"
						" number %d to station"
						" A. Aborting.\n", k);
				goto out;
			}
			free(time_input);
		}
		/*
		printf("Printing start table from A to B:\n");
		tt_avl_lvr(a_start_time);
		printf("\nDone\n");
		printf("Printing end table from A to B:\n");
		tt_avl_lvr(a_end_time);
		printf("\nDone\n");
		*/
		// Trips from station B to A
		for (k = 0; k < trips_from_b; k++) {
			int str_len = 0;
			unbalanced_b_start = 0;
			unbalanced_b_end = 0;
			time_input = malloc(MAX_SIZE * sizeof(char));
			tst = malloc(sizeof(struct train_start_time));
			tet = malloc(sizeof(struct train_end_time));
			memset(time_input, '\0', MAX_SIZE);
			if (!time_input) {
				printf("Failed to allocate memory for input "
						"time. Aborting.\n");
			}
			fgets(time_input, MAX_SIZE, stdin);
			str_len = strlen(time_input);
			time_input[str_len] = '\0';

			tok = strtok(time_input, ":");
			tst->start_hour = atoi(tok);
			tok = strtok(NULL, " :");
			tst->start_min = atoi(tok);
			tok = strtok(NULL, " :");
			tet->end_hour = atoi(tok);
			tok = strtok(NULL, "");
			tet->end_min = atoi(tok);
			if (tt_insert_node(&b_start_time, tst, tet, 1,
						&unbalanced_b_start)) {
				printf("Failed to add node for schedule"
						" number %d from station"
						" A. Aborting.\n", k);
				goto out;
			}
			if (tt_insert_node(&b_end_time, tst, tet, 0,
						&unbalanced_b_end)) {
				printf("Failed to add node for schedule"
						" number %d from station"
						" A. Aborting.\n", k);
				goto out;
			}
			free(time_input);
		}
		/*
		printf("Printing start table from B to A:\n");
		tt_avl_lvr(b_start_time);
		printf("\nDone\n");
		printf("Printing end table from B to A:\n");
		tt_avl_lvr(b_end_time);
		printf("\nDone\n");
		*/

		//Final count of trains from A to B
		count_a = trips_from_a;
		get_count(a_start_time, b_end_time, &count_a, turn_around_time);

		//Final count of trains from B to A
		count_b = trips_from_b;
		get_count(b_start_time, a_end_time, &count_b, turn_around_time);

		printf("Case #%d: %d %d\n", i + 1, count_a, count_b);
	}


	ret = 0;
out:
	return ret;
}
