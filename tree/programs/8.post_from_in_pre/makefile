CC = gcc
CFLAGS = -Wall -g -I../../practice/headers
TARGET = generate_post
SRCS = generate_post.c
OBJS = $(SRCS:.c=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)
