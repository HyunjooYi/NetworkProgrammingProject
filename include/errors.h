#ifndef __NPP_ERROR__
#define __NPP_ERROR__

/* 01~99 : system error */
#define NPP0001 "Memory allocation failed"
#define NPP0002 "fcntl falled"
#define NPP0003 "write failed"
#define NPP0004 "select failed"

/* 100~999 : client error */
#define NPP0100 "socket of client failed"
#define NPP0101 "connect to server failed"

/* 1000~1999 : server error */
#define NPP1000 "socket of server failed"

/* 2000~ : distingush for error code line */

#endif