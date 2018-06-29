CC = gcc
IDIR = ../../headers
CFLAGS = -Wall -g -I$(IDIR)
TARGET = triplets
SRCS = triplets.c
OBJS = $(SRCS:.c=.o)
HEADERS = -I../../headers/
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)
