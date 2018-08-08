#ifndef __USRINC__
#define __USRINC__

#include "../include/common.h"
#include "../client/client_conn.h"

/* definitions */
#define NPP_OK	1 /* success */
#define NPP_ERROR -1 /* fail(error) */
/* ----- */
#define NPP_ESYSTEM 2 /* system call error */
#define NPP_EMAXNO 3 /* max errno */

/* functions */
char* npp_str_err(int _errno);
int npp_svccall(int conn, char *svcname, char *send_buffer, int sndlen, char **recv_buffer, int &recvlen, int flags);

int npperrno;

#endif