#include "server.h"

void init() {
	FD_ZERO(&readfds);
	FD_ZERO(&allfds);
	listen_fd = INITFD;
	client_fd = INITFD;
	maxfd = INITFD;
}

int main (void) {
	// 1. init args in server/socket
	init();

	// 2. make socket & fd_set
	listen_fd = init_listen_fd();
	maxfd = listen_fd;

	// 3. loop
	loop();

	// 4. close listen fd
	close(listen_fd);

	return 0;
}

void loop() {
	fd_set allfds;
	struct sockaddr_in client_addr;
	int len, i, readn, n;
	char recv_data[BUF_SIZE], send_data[BUF_SIZE];

	memset(&send_data, 0x00, BUF_SIZE);
	memset(&recv_data, 0x00, BUF_SIZE);

	while(1) {
		allfds = readfds;
		n = select(maxfd + 1, &allfds, NULL, NULL, NULL);
		if(n > 0) {
			if(FD_ISSET(listen_fd, &allfds) != 0) {
				len = sizeof(client_addr);
				client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, (socklen_t *)&len);
				if(client_fd < 0) {
					printf("[%s] %s\n", "NPP2012", NPP1003);
					npperrno = NPP_ESYSTEM;
					return;
				}

				FD_SET(client_fd, &readfds);
				if(npp_chfd_nonblocking(client_fd) < 0) {
					printf("[%s] %s\n", "NPP2013", NPP0002);
					npperrno = NPP_ESYSTEM;
					return;
				}
				if(client_fd > maxfd)
					maxfd = client_fd;
				printf("Accepted\n");
				continue;
			}
			if(FD_ISSET(client_fd, &allfds) != 0) {
				i = 0;
				/* TODO : ***** 읽어올 길이를 header를 통해서 미리 알아야 한다. 지금은 그냥 고정 버퍼 길이로 테스트 겸 해봄 ****** */
				while((readn = read(client_fd, recv_data, BUF_SIZE)) > 0) {
					for(; recv_data[i]; i++)
						send_data[i] = recv_data[i] - 32;

					send_data[i] = '\0';
					printf("send : %s\n", send_data);					
					/* TODO : ***** 보낼 길이도 header를 통해서 미리 client 쪽에게 알려하 한다. 이것도 지금은 그냥 고정 버퍼 길이로 테스트 겸 해봄 ****** */
					write(client_fd, send_data, BUF_SIZE);
				}
				if(readn == 0) {
					close(client_fd);
					FD_CLR(client_fd, &readfds);
					printf("client closed\n");
				} else if (readn == -1) {
					if(errno != EAGAIN) {
						printf("[%s] %s\n", "NPP2014", NPP0004);
						npperrno = NPP_ESYSTEM;
						return;
					}
				}
			}
		}
	}
}

int init_listen_fd() {
	struct sockaddr_in server_addr;
	int fd;

	if((fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		printf("[%s] %s\n", "NPP2009", NPP1000);
		npperrno = NPP_ESYSTEM;
		return NPP_ERROR;
	}

	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	if(bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		printf("[%s] %s\n", "NPP2010", NPP1001);
		npperrno = NPP_ESYSTEM;
		return NPP_ERROR;
	}

	if(listen(fd, 5) < 0) {
		printf("[%s] %s\n", "NPP2011", NPP1002);
		npperrno = NPP_ESYSTEM;
		return NPP_ERROR;
	}

	FD_SET(fd, &readfds);

	return fd;
}
