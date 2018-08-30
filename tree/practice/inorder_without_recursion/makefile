CC = gcc
CFLAGS = -Wall -g -I../headers/ -I../generic/
TARGET = inorder_without_recursion
SRCS = inorder_without_recursion.c
OBJS = $(SRCS:.c=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(OBJS) $(TARGET)
