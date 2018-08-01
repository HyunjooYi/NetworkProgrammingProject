#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>
#define PORT 1234
#define BUF_SIZE 1024

void nonblock(int client_fd) {
	int opts;
	opts = fcntl(client_fd, F_GETFL);
	if(opts < 0) {
		printf("fcntl(F_GETFL) error\n");
		exit(0);
	}
	opts = (opts | O_NONBLOCK);
	if(fcntl(client_fd, F_SETFL, opts) < 0) {
		printf("fcntl(F_SETFL) error\n");
		exit(0);
	}
}

int main (void) {
	struct sockaddr_in server_addr, client_addr;
	int listen_fd, client_fd, len, i, size;
	char recv_data[BUF_SIZE], send_data[BUF_SIZE];
	memset(&send_data, 0x00, BUF_SIZE);
	memset(&recv_data, 0x00, BUF_SIZE);
	int readn;
	int maxfd;
	int n;
	fd_set readfds, allfds;

	if((listen_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		printf("error cannnot open server\n");
		exit(0);
	}

	memset(&server_addr, 0x00, sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);

	if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		printf("error cannot bind local address\n");
		exit(0);
	}

	if(listen(listen_fd, 5) < 0) {
		printf("error cannot listen connection\n");
		exit(0);
	}

	FD_ZERO(&readfds);
	FD_SET(listen_fd, &readfds);
	maxfd = listen_fd;

	while(1) {
		allfds = readfds;
		n = select(maxfd + 1, &allfds, NULL, NULL, NULL);
		if(n > 0) {
			if(FD_ISSET(listen_fd, &allfds) != 0) {
				len = sizeof(client_addr);
				client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, (socklen_t *)&len);
				if(client_fd < 0) {
					printf("error cannot accept client\n");
					exit(0);
				}

				FD_SET(client_fd, &readfds);
				nonblock(client_fd);
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
						close(client_fd);
						FD_CLR(client_fd, &readfds);
						printf("client closed\n");
					}
				}
			}
		}
	}

	close(listen_fd);

	return 0;
}
