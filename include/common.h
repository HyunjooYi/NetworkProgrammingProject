#ifndef __COMMON__
#define __COMMON__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include "npc_err.h"
#include "errors.h"

/* definitions */
#define NPP_OK	1 /* success */
#define NPP_ERROR -1 /* fail(error) */

#define NPP_ESYSTEM 2 /* system call error */

/* extern valiables */
extern int npperrno;

/* functions */
void npp_error(int level, char *errnum);
void npp_chfd_nonblocking(int fd);

#endif