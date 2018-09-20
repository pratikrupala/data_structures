CC = gcc
CFLAGS = -Wall -g -I../../practice/generic -I../../practice/headers
TARGET = boundary
SRCS = boundary.c
OBJS = $(SRCS:.c=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)
