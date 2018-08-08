#include "common.h"

int npp_chfd_nonblocking(int fd) {
	int opts;

	if((opts = fcntl(fd, F_GETFL)) < 0) {
		/* TODO : change error print to npp_error */
		printf("%s %s\n", "NPP2003", NPP0002);
		npperrno = NPP_ESYSTEM;
		return NPP_ERROR;
	}

	opts |= O_NONBLOCK;
	if(fcntl(fd, opts, F_SETFL) < 0) {
		/* TODO : change error print to npp_error */
		printf("%s %s\n", "NPP2004", NPP0002);
		npperrno = NPP_ESYSTEM;
		return NPP_ERROR;
	}

	return NPP_OK;
}
