CC = gcc
CFLAGS = -Wall -g
LIBS = -lrt -pthread
TARGET = $(TARGET1) $(TARGET2)
TARGET1 = writer
TARGET2 = reader
COMMON = write_helper.c
SRCS1 = writer.c
SRCS2 = reader.c
OBJS1 = $(SRCS1:.c=.o)
OBJS2 = $(SRCS2:.c=.o)
COMMONOBJS = $(COMMON:.c=.o)
RM = rm

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJS1) $(COMMONOBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS1) $(COMMONOBJS) $(LIBS)

$(TARGET2): $(OBJS2) $(COMMONOBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS2) $(COMMONOBJS) $(LIBS)

clean:
	$(RM) $(TARGET1) $(TARGET2) $(OBJS1) $(OBJS2) $(COMMONOBJS) /dev/shm/lk_sl* /dev/shm/sbuf* /dev/shm/sbloc*
