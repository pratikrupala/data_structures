struct tree* stk[MAX_LEN];
int top = -1;

int
push(struct tree *data)
{
	if (top == (MAX_LEN - 1)) {
		printf("\nStack is full\n");
		return -1;
	}
	stk[++top] = data;

	return 0;
}

struct tree *
pop(void)
{
	if (top < 0) {
		return NULL;
	}
	return stk[top--];
}
