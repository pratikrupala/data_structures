CC = g++
CFLAGS = -Wall -g
TARGET = min_sub_sum
SRCS = min_sub_sum.cpp
OBJS = $(SRCS:.cpp=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(TARGET) $(OBJS)
