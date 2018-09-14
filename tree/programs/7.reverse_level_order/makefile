CC = gcc
CFLAGS = -Wall -g -I../../practice/headers -I../../practice/generic
TARGET = reverse_level
SRCS = reverse_level.c
OBJS = $(SRCS:.c=.o)
RM = rm

all: $(TARGET)
	
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)
