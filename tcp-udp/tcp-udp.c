#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <errno.h>
#include "../log/log.h"

#define MAXSIZE 1000
#define BACKLOG 5

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

void tcp_client(struct sockaddr_in *server_addr)
{
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1)
	{
		perror("socket");
		log_perror("socket\n");
		exit(EXIT_FAILURE);
	}

	int optval = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
	{
		perror("setsockopt");
		log_perror("setsockopt\n");
		exit(EXIT_FAILURE);
	}

	if (connect(sock_fd, (struct sockaddr *)server_addr,
				sizeof(struct sockaddr)) == -1)
	{
		perror("connect");
		log_perror("connect\n");
		exit(EXIT_FAILURE);
	}

	char buf[MAXSIZE];
	while (1)
	{
		fflush(stdin);
		fgets(buf, MAXSIZE, stdin);
		write(sock_fd, buf, MAXSIZE);
	}
	close(sock_fd);
}

void udp_client(struct sockaddr_in *server_addr)
{
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd == -1)
	{
		perror("socket");
		log_perror("socket\n");
		exit(0);
	}

	int optval = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
	{
		perror("setsockopt");
		log_perror("setsockopt\n");
		exit(EXIT_FAILURE);
	}

	char buf[MAXSIZE];
	while (1)
	{
		fflush(stdin);
		fgets(buf, MAXSIZE, stdin);

		if (sendto(sock_fd, &buf, sizeof(buf), 0,
				   (struct sockaddr *)server_addr,
				   sizeof(struct sockaddr_in)) == -1)
		{
			perror("sendto");
			log_perror("sendto\n");
			exit(EXIT_FAILURE);
		}
	}
	close(sock_fd);
}

void tcp_server(struct sockaddr_in *server_addr)
{
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1)
	{
		perror("socket");
		log_perror("sendto\n");
		exit(EXIT_FAILURE);
	}

	int optval = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1)
	{
		perror("setsockopt");
		log_perror("setsockopt\n");
		exit(EXIT_FAILURE);
	}

	if (bind(sock_fd, (struct sockaddr *)server_addr,
			 sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		log_perror("bind\n");
		exit(EXIT_FAILURE);
	}

	if (listen(sock_fd, BACKLOG) == -1)
	{
		perror("listen");
		log_perror("listen\n");
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
		log_perror("accept\n");
		exit(EXIT_FAILURE);
	}

	while (read(cfd, buf, MAXSIZE))
	{
		printf("message from client: %s", buf);
	}
	close(cfd);
	close(sock_fd);
}

void udp_server(struct sockaddr_in *server_addr)
{
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd == -1)
	{
		perror("socket");
		log_perror("socket\n");
		exit(EXIT_FAILURE);
	}

	if (bind(sock_fd, (struct sockaddr *)server_addr,
			 sizeof(struct sockaddr_in)) == -1)
	{
		perror("bind");
		log_perror("bind\n");
		exit(EXIT_FAILURE);
	}

	char buf[MAXSIZE];
	socklen_t addrlen = sizeof(buf);
	struct sockaddr_in client_addr;

	while (1)
	{
		if (recvfrom(sock_fd, buf, sizeof(buf), 0,
					 (struct sockaddr *)&client_addr,
					 &addrlen) == -1)
		{
			continue;
		}

		char *ip = inet_ntoa(client_addr.sin_addr);
		printf("message from client %s:\n%s", ip, buf);
	}

	close(sock_fd);
}