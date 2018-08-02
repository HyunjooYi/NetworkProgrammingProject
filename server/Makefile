CC = gcc
CFLAGS = -W -Wall
TARGET = server
OBJECTS = server.o ../include/common.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean :
	rm *.o server