#include <stdio.h>
#include <stdlib.h>
#include <circular_queue.h>

struct lnode {
	char bot_color;
	int bot_pos;
	int seq_no;
	struct lnode * next;
};

struct lnode *orange_head = NULL;
struct lnode *orange_last = NULL;
struct lnode *blue_head = NULL;
struct lnode *blue_last = NULL;

int list_add(char c, int pos, int *seq)
{
	int res = -1;
	struct lnode *new_node = malloc(sizeof(struct lnode));
	if (!new_node) {
		printf("\nFailed to allocate memory for %c %d.\n", c, pos);
		goto out;
	}
	new_node->bot_color = c;
	new_node->bot_pos = pos;
	new_node->seq_no = ++(*seq);
	new_node->next = NULL;

	if (c == 'O') {
		if (orange_head) {
			orange_last->next = new_node;
			orange_last = orange_last->next;
		} else {
			orange_head = new_node;
			orange_last = new_node;
		}
	} else if (c == 'B') {
		if (blue_head) {
			blue_last->next = new_node;
			blue_last = blue_last->next;
		} else {
			blue_head = new_node;
			blue_last = new_node;
		}
	} else {
		free(new_node);
		--(*seq);
	}

out:
	return res;
}

void iter_list(char c) {
	struct lnode *iter = NULL;
	if (c == 'O')
		iter = orange_head;
	else
		iter = blue_head;
	while (iter) {
		printf("%d %c %d\t", iter->seq_no, iter->bot_color,
			iter->bot_pos);
		iter = iter->next;
	}
	printf("\n");
}

int calculate_steps(void)
{
	int steps = 0;
	int o_cur_pos = 1;
	int b_cur_pos = 1;
	struct lnode *o_ptr = orange_head;
	struct lnode *b_ptr = blue_head;
	struct lnode *primary_ptr = NULL;
	struct lnode *secondary_ptr = NULL;

	while (o_ptr && b_ptr) {
		int primary_diff = 0;
		int primary_cur_pos = 0;
		int secondary_diff = 0;
		int secondary_cur_pos = 0;
		int primary_pos = 0;
		int secondary_pos = 0;
		int flag = 0;
		if (o_ptr->seq_no < b_ptr->seq_no) {
			primary_ptr = o_ptr;
			primary_cur_pos = o_cur_pos;
			secondary_ptr = b_ptr;
			secondary_cur_pos = b_cur_pos;
			flag = 1;
		} else {
			primary_ptr = b_ptr;
			primary_cur_pos = b_cur_pos;
			secondary_ptr = o_ptr;
			secondary_cur_pos = o_cur_pos;
		}

		primary_diff = abs(primary_ptr->bot_pos - primary_cur_pos);
		steps += primary_diff;
		steps++;
		primary_cur_pos = primary_ptr->bot_pos;
		primary_pos = 1;
		secondary_diff = abs(secondary_ptr->bot_pos
			- secondary_cur_pos);
		if (primary_diff >= secondary_diff) {
			secondary_cur_pos = secondary_ptr->bot_pos;
		} else {
			if (secondary_cur_pos < secondary_ptr->bot_pos) {
				secondary_cur_pos += (primary_diff + 1);
			} else {
				secondary_cur_pos -= (primary_diff + 1);
			}
		}

		if (flag) {
			o_cur_pos = primary_cur_pos;
			b_cur_pos = secondary_cur_pos;
			if (primary_pos)
				o_ptr = o_ptr->next;
			if (secondary_pos)
				b_ptr = b_ptr->next;
		} else {
			b_cur_pos = primary_cur_pos;
			o_cur_pos = secondary_cur_pos;
			if (primary_pos)
				b_ptr = b_ptr->next;
			if (secondary_pos)
				o_ptr = o_ptr->next;
		}
	}

	struct lnode *ptr = NULL;
	int cur_pos = 0;
	if (o_ptr) {
		ptr = o_ptr;
		cur_pos = o_cur_pos;
	} else {
		ptr = b_ptr;
		cur_pos = b_cur_pos;
	}

	while (ptr) {
		steps += abs(ptr->bot_pos - cur_pos);
		steps++;
		cur_pos = ptr->bot_pos;
		ptr = ptr->next;
	}

	return steps;
}

int main(void)
{
	int i = 0;
	int tc = -1;

	scanf("%d", &tc);
	if ((tc < 1) || (tc > 100)) {
		printf("Invalid number of test cases.\n");
		goto out;
	}

	for (i = 0; i < tc; i++) {
		int k = 0;
		int tmp = 0;
		int len = 0;
		int steps = 0;
		char c;
		int pos;

		scanf("%d", &tmp);
		if ((tmp < 1) || (tmp > 100)) {
			printf("Invalid number of sequences.");
			goto out;
		}
		len = tmp * 2;

		orange_head = NULL;
		orange_last = NULL;
		blue_head = NULL;
		blue_last = NULL;
		int *seq = NULL;
		seq = malloc(sizeof(int));
		*seq = 0;
		for (k = 0; k < len; k++) {
			scanf("%c", &c);
			scanf("%d", &pos);
			list_add(c, pos, seq);
		}

		/*
		iter_list('O');
		iter_list('B');
		*/

		steps = calculate_steps();
		printf("Case #%d: %d\n", i + 1, steps);
	}
out:
	return 0;
}
