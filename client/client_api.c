#include "client_conn.h"
#include "../include/common.h"

int npp_svccall(int conn, char *svcname, char *send_buffer, int sndlen, char **recv_buffer, int *recvlen, int flags) {
	header_t *hp;
	int ret;

	hp = (header_t *) malloc(HEADER_SIZE + sndlen);

	hp->flags = flags;
	hp->conn = conn;
	strcpy(hp->msgtype, svcname);
	hp->data_size = sndlen;
	hp->asize = sndlen + HEADER_SIZE;
	memcpy(hp + sndlen, send_buffer, sndlen);

	if((ret = nppc_send_msg(hp)) < 0)
		return NPP_ERROR;
	
	if((ret = nppc_recv_msg(&hp)) < 0) 
		return NPP_ERROR;

	if(hp->rcode != NPP_OK) {
		npperrno = hp->rcode;
		return NPP_ERROR;
	}

	strncpy(*recv_buffer, hp + sizeof(header_t), hp->data_size);
	*recvlen = hp->data_size;

	return NPP_OK;
}