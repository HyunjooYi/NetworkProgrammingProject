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
		printf("failed to get connection : npperrno[%d] : %s\n", npperrno, npp_str_err(npperrno));
		return -1;
	}
	printf("successfully got a connection\n");
	strcpy(send_data, argv[1]);
	len = BUF_SIZE;

	if(npp_svccall(conn, "TOUPPER", send_data, len, (char **) &recv_data, &len, flags) < 0) {
		printf("failed to call svc[%s] according to %d[%s]\n", "TOUPPER", npperrno, npp_str_err(npperrno));
		return -1;
	}
	printf("successfully send[%s] and recv[%s] data\n", send_data, recv_data);

	if(nppc_close_conn(conn, flags) < 0) {
		printf("failed to close connection id:[%d]\n", conn);
		return -1;
	}
	printf("successfully close connection id:[%d]\n", conn);

	return 0;
}
