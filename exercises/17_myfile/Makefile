CC = gcc
CFLAGS = -Wall -g -O2 -fPIE

TARGET := 17_myfile
SRCS := 17_myfile.c
OBJS := $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)
	$(CC) $(CFLAGS) -c -o $(TARGET).o $(SRCS)


clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean