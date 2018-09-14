#include <stdio.h>

struct tree *cq[MAX_LEN];
int front = -1;
int rear = -1;

void
reset_queue(void)
{
	front = -1;
	rear = -1;
}

void
addq(struct tree *element)
{
	if (((front == -1) && ((rear + 1) == MAX_LEN - 1)) ||
		       (((rear + 1) % MAX_LEN) == front)) {
		printf("\nQueue is full.\n");
		return;
	}
	rear = (rear + 1) % MAX_LEN;
	cq[rear] = element;
}

struct tree*
delq(void)
{
	if ((rear == -1) || (rear == front)) {
//		printf("\nQueue is empty.\n");
		return NULL;
	}
	front++;
	return cq[front];
}
