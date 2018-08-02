#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../usrinc/usrinc.h"

int main(int argc, char *argv[]) {
	int conn, flags, len;
	char send_data[BUF_SIZE], recv_data[BUF_SIZE];

	if(argc != 2) {
		printf("Usage : ./client text\n");
		return -1;
	}

	/* TODO: change flags to flags |= NPP_NO_FLAGS; */
	flags = 0;
	if((conn = nppc_open_conn(flags)) < 0) {
		printf("failed to get connection : npperrno[%d]\n", npperrno);
		return -1;
	}

	printf("successfully got a connection\n");
	strcpy(send_data, argv[1]);
	len = BUF_SIZE;
	/* TODO : seperate all used flags in this file */
	if(nppc_send_msg(conn, send_data, len, flags) < 0) {
		printf("failed to send msg to server : npperrno[%d]", npperrno);
		return -1;
	}
	printf("successfully send msg[%s] to server\n", send_data);

	if(nppc_recv_msg(conn, (char **)&recv_data, &len, flags) < 0) {
		printf("failed to recv msg from server : npperrno[%d]\n", npperrno);
		return -1;
	}
	printf("successfully recv msg[%s] from server\n", recv_data);

	if(nppc_close_conn(conn, flags) < 0) {
		printf("failed to close connection[%d]\n", conn);
		return -1;
	}
	printf("successfully close connection[%d]\n", conn);

	return 0;
}
