#ifndef __SERVER__
#define __SERVER__

#include "../include/common.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* definitions */
#define INITFD -1
#define PORT 1234
#define BUF_SIZE 1024

/* variables */
int npperrno;
fd_set readfds;
fd_set allfds;
int listen_fd;
int client_fd;
int maxfd;

/* functions */
void init();
void loop();
int init_listen_fd();
#endif