CC = gcc
CFLAGS = -Wall -g -I../../practice/headers -I../../practice/generic
TARGET = generate
SRCS = generate.c
OBJS = $(SRCS:.c=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)
