client : client.o client_conn.o ../include/common.o
	gcc -o client client.o client_conn.o ../include/common.o

client.o : client.c
	gcc -c -o client.o client.c

client_conn.o : client_conn.c
	gcc -c -o client_conn.o client_conn.c

common.o : ../include/common.c
	gcc -c -o ../include/common.o ../include/common.c

clean :
	rm *.o client