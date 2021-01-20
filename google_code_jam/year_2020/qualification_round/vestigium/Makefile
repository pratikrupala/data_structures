CC = g++
CFLAGS = -Wall -g
TARGET = vestigium
SRCS = retry.cpp
OBJS = $(SRCS:.cpp=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(OBJS) $(TARGET)
