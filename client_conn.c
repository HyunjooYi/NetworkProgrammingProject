#include "client_conn.h"

int nppc_recv_msg(int conn, char **_recv_msg, int &len, int flags) {
	int readn, left, n, offset;
	int phase = 0;
	struct timeval time;
	char *recv_msg;

	time.tv_sec = 1;
	time.tv_usec = 0;
	left = BUF_SIZE; /* TODO : ***** 읽어올 길이를 header를 통해서 미리 알아야 한다. 지금은 그냥 고정 버퍼 길이로 테스트 겸 해봄 ****** */
	offset = 0;
	recv_msg = *_recv_msg;
	
	while(1) {
		if((n = select(conn, &readfds, NULL, NULL, &time)) < 0) {
			if(n != EAGAIN) {
				printf("%s %s\n", "NPP2007", NPP0104);
				npperrno = NPP_ESYSTEM;
				return NPP_ERROR;
			}
		} else {
			if(FD_ISSET(conn, &readfds) != 0) {
				if((readn = read(conn, recv_msg + offset, left)) < 0) {
					printf("%s %s\n", "NPP2008", NPP0103);
					npperrno = NPP_ESYSTEM;
					return NPP_ERROR;
				}
				if(phase == 0) {
					left = len - readn;
					/* TODO : 나중에 여러 client가 붙으면 응답이 섞일 수 있음
					 나에게로 온 응답이 아니면 unmatched_process를 태우고 끝내야 한다
					 그때는 left가 음수일 수도 있을 것 같음 */
					if(left == 0)
						break;
					offset = readn;
					phase = 1;
				} else if(phase == 1) {
					left -= readn;
					/* TODO : 나중에 여러 client가 붙으면 응답이 섞일 수 있음
					 나에게로 온 응답이 아니면 unmatched_process를 태우고 끝내야 한다
					 그때는 left가 음수일 수도 있을 것 같음 */
					if(left == 0)
						break;
					offset += readn;
				}
			}
		}
	}

	return NPP_OK;
}

int nppc_send_msg(int conn, char *send_buf, int len, int flags) {
	int writen, left, n, offset;
	int phase = 0;
	struct timeval time;

	time.tv_sec = 1;
	time.tv_usec = 0;

	left = len;
	offset = 0;
	while(1) {
		if((n = select(conn, NULL, &writefds, NULL, &time)) < 0) {
			if(n != EAGAIN) {
				printf("%s %s\n", "NPP2006", NPP0104);
				npperrno = NPP_ESYSTEM;
				return NPP_ERROR;
			}
		} else {
			if(FD_ISSET(conn, &writefds) != 0) {
				if((writen = write(conn, send_buf+offset, left)) < 0) {
					printf("%s %s\n", "NPP2005", NPP0103);
					npperrno = NPP_ESYSTEM;
					return NPP_ERROR;
				}
				if(phase == 0) {
					left = len - writen;
					if(left == 0)
						break;
					offset = writen;
					phase = 1;
					continue;
				} else if(phase == 1) {
					left -= writen;
					if(left == 0)
						break;
					offset += writen;
				}
			}
		}
	}

	return NPP_OK;
}

int nppc_open_conn(int flags) {
	int client_fd;
	struct sockaddr_in server_addr;

	/* get socket fd */
	if((client_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		/* TODO : change error print to npp_error */
		printf("%s %s\n", "NPP2001", NPP0100);
		/* TODO : change npperrno as thread valiable */
		npperrno = NPP_ESYSTEM;
		return NPP_ERROR;
	}

	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	server_addr.sin_addr.s_addr = inet_addr(IP);
	server_addr.sin_port = htons(PORT);

	/* get connection from socket fd */
	if(connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		/* TODO : change error print to npp_error */
		printf("%s %s\n", "NPP2002", NPP0101);
		/* TODO : change npperrno as thread valiable */
		npperrno = NPP_ESYSTEM;
		nppc_close_conn(client_fd, 0);
		return NPP_ERROR;
	}

	if (npp_chfd_nonblocking(client_fd) < 0) {

	}
	FD_CLR(&writefds);
	FD_CLR(&readfds);
	FD_SET(client_fd, &writefds);
	FD_SET(client_fd, &readfds);

	return client_fd;
}

int nppc_close_conn(int conn, int flags) {
	FD_CLR(conn, &writefds);
	FD_CLR(conn, &readfds);
	close(conn);

	return NPP_OK;
}