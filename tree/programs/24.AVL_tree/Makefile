CC = gcc
CFLAGS = -Wall -g
TARGET = avl
SRCS = avl.c
OBJS = $(SRCS:.c=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(OBJS) $(TARGET)
