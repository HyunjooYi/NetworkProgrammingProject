#ifndef __COMMON__
#define __COMMON__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include "errors.h"
#include "../usrinc/usrinc.h"

/* extern valiables */
extern int npperrno;

/* structs */
/* npp header : must be 16 byte alignment (for 32, 64bit) */
typedef struct header_s {
	int magic;
	int conn; // socket fd
	char seqno[8]; // msg sequence number. 8bytes = 64bit
	int session_id; // session id for sharing connection
	int clid; // client id
	int asize; // header size + data size
	int data_size; // real data size
	int msgtype; // type for distingush the services
	int rcode; // return code. usually used for errcode
	int flags;
	int etc; // for exceptional information
} header_t;

/* functions */
void npp_error(int level, char *errnum);
int npp_chfd_nonblocking(int fd);

#endif