CC = gcc
CFLAGS = -Wall -g -I../../practice/headers -I../../practice/generic
TARGET = top_view
SRCS = top_view.c
OBJS = $(SRCS:.c=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)
