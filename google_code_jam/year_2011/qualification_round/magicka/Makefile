CC = gcc
CFLAGS = -Wall -g -I.
TARGET = magicka
SRCS = magicka.c avl_support.c
OBJS = $(SRCS:.c=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(OBJS) $(TARGET)
