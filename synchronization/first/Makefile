CC = gcc
CFLAGS = -Wall -g
LIBS1 = -lrt -pthread
LIBS2 = -lrt -pthread -lm
TARGET = $(TARGET1) $(TARGET2)
TARGET1 = writer
TARGET2 = reader
COMMON = common.c
COMMON1 = write_helper.c $(COMMON)
COMMON2 = read_helper.c $(COMMON)
SRCS1 = writer.c
SRCS2 = reader.c
OBJS1 = $(SRCS1:.c=.o)
OBJS2 = $(SRCS2:.c=.o)
COMMONOBJS1 = $(COMMON1:.c=.o)
COMMONOBJS2 = $(COMMON2:.c=.o)
RM = rm -f

all: $(TARGET1) $(TARGET2)

debug: CFLAGS += -DDEBUG
debug: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJS1) $(COMMONOBJS1)
	$(CC) $(CFLAGS) -o $@ $(OBJS1) $(COMMONOBJS1) $(LIBS1)

$(TARGET2): $(OBJS2) $(COMMONOBJS2)
	$(CC) $(CFLAGS) -o $@ $(OBJS2) $(COMMONOBJS2) $(LIBS2)

clean:
	$(RM) $(TARGET1) $(TARGET2) $(OBJS1) $(OBJS2) $(COMMONOBJS1) $(COMMONOBJS2) /dev/shm/sbuf* /dev/shm/sbloc* output.txt
