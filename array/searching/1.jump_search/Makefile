CC = gcc
CFLAGS = -Wall -g
LIBS = -lm
TARGET = jump_search
SRCS = jump.c
OBJS = $(SRCS:.c=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	$(RM) $(TARGET) $(OBJS)
