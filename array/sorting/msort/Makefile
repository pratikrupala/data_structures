CC = gcc
CFLAGS = -Wall -g
TARGET = msort
SRCS = msort.c
OBJS = $(SRCS:.c=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)
