CC = gcc
CFLAGS = -W -Wall
TARGET = client
OBJECTS = client.o client_conn.o ../include/common.o ../include/npp_error.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean :
	rm *.o client