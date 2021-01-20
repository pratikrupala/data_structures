CC = g++
CFLAGS = -Wall -g
TARGET = nest_depth
SRCS = nest_depth.cpp
OBJS = $(SRCS:.cpp=.o)
RM = rm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

clean:
	$(RM) $(OBJS) $(TARGET)
