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
const int MAXSIZE = 1000;

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
	while(protocol != 1 && protocol != 2)
	{
		printf("Wrong number\n");
		printf("Enter 1 for TCP, 2 for UDP: ");
		scanf("%d", &protocol);
	}

	return protocol;
}

void tcp(struct sockaddr_in *server_addr)
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
	
	if (connect(sock_fd, (struct sockaddr *)server_addr,
				 sizeof(struct sockaddr)) == -1)
	{
		perror("connect");
		exit(EXIT_FAILURE);
	}

	char buf[MAXSIZE];
	while(1)
	{
		fflush(stdin);
		fgets(buf, MAXSIZE, stdin);
		write(sock_fd, buf, MAXSIZE);
	}
	close(sock_fd);
}

void udp(struct sockaddr_in *server_addr)
{
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd == -1)
	{
		perror("socket");
		exit(0);
	}

	int optval = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	char buf[MAXSIZE];
	while(1)
	{
		fflush(stdin);
		fgets(buf, MAXSIZE, stdin);

		if (sendto(sock_fd, &buf, sizeof(buf), 0,
				(struct sockaddr *)server_addr,
				sizeof(struct sockaddr_in)) == -1)
		{
			perror("sendto");
			exit(EXIT_FAILURE);
		}
	}
}