CC = gcc
CFLAGS = -Wall -g -I../../headers/ -I../../generic/
TARGET = lorder
SRCS = lorder.c
OBJS = $(SRCS:.c=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)
