CC = gcc
CFLAGS = -Wall -g -I../../practice/headers -I../../practice/generic
TARGET = traverse
SRCS = traverse.c
OBJS = $(SRCS:.c=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)
