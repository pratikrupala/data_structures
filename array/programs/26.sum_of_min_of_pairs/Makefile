CC = g++
CFLAGS = -Wall -g
TARGET = pair_sum
SRCS = pair_sum.cpp
OBJS = $(SRCS:.cpp=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)
