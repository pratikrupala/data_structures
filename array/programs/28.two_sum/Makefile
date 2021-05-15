CC = g++
CFLAGS = -Wall -g
TARGET = two_sum
SRCS = two_sum.cpp
OBJS = $(SRCS:.cpp=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)
