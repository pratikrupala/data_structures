CC = gcc
CFLAGS = -Wall -g
TARGET = train_time_table
SRCS = train_time.c avl_support.c
OBJS = $(SRCS:.c=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)
