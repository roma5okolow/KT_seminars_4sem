#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

int select_protocol(void);
void tcp(struct sockaddr_in *server_addr);
void udp(struct sockaddr_in *server_addr);

const int MY_SOCK_PATH = 10001;
const int BACKLOG = 5;
const int MAXSIZE = 100000;

int main()
{
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(MY_SOCK_PATH);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (select_protocol() == 1)
	{
		printf("TCP selected\n");
		tcp(&server_addr);
	}

	else
	{
		printf("UDP selected\n");
		udp(&server_addr);
	}
}

int select_protocol(void)
{
	int protocol = 0;
	printf("Enter 1 for TCP, 2 for UDP: ");
	scanf("%d", &protocol);
	while (protocol != 1 && protocol != 2)
	{
		printf("Wrong number\n");
		printf("Enter 1 for TCP, 2 for UDP: ");
		scanf("%d", &protocol);
	}

	return protocol;
}

void tcp(struct sockaddr_in* server_addr)
{

	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	int optval = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	if (bind(sock_fd, (struct sockaddr *)server_addr,
			 sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(sock_fd, BACKLOG) == -1)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in peer_addr;
	socklen_t peer_addr_size = sizeof(struct sockaddr_in);
	
	char buf[MAXSIZE];

	int cfd = accept(sock_fd, (struct sockaddr *)&peer_addr,
				 &peer_addr_size);
	if (cfd == -1)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	while (read(cfd, buf, MAXSIZE))
	{
		printf("message from client: %s", buf);	
	}
	close(cfd);
}

void udp(struct sockaddr_in *server_addr)
{
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd == -1)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	if (bind(sock_fd, (struct sockaddr *)server_addr,
			 sizeof(struct sockaddr_in)) == -1)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	char buf[MAXSIZE];
	socklen_t addrlen = sizeof(buf);
	struct sockaddr_in src_addr;
	
	while(1)
	{
		if (recvfrom(sock_fd, buf, sizeof(buf), 0,
					(struct sockaddr *)&src_addr,
					&addrlen) == -1)
		{
			continue;
		}

		char *ip = inet_ntoa(src_addr.sin_addr);
		printf("message from client %s\n%s", ip, buf);
	}
}