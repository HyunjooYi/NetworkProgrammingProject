#include "common.h"

char *_npp_errtab[] = {
	"",	/* 0 */
	"", /* 1 */
	"system call error(NPP_ESYSTEM)",	/* 2 */
	""  
};

/* TODO : vsprintf(), ap... */
void npp_error(int level, char *errnum) {
		
}

char *npp_str_err(int _errno) {
	if(_errno < 0 || _errno > NPP_EMAXNO)
		_errno = NPP_EMAXNO;

	return _npp_errtab[_errno];
}