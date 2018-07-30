#ifndef __CLIENT_CONN__
#define __CLIENT_CONN__

#include "include/common.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define PORT 1234
#define IP "127.0.0.1"

/* valialbes */
int npperrno;
/* for clinet I/O Multiplexing */
int writefds;
int readfds;

/* functions */
int nppc_open_conn(int flags);
int nppc_close_conn(int conn, int flags);
int nppc_send_msg(int conn, char *send_buf, int len, int flags);
int nppc_recv_msg(int conn, char **recv_buf, int len, int flags);

#endif