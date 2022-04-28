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

#define MY_SOCK_PATH 27312
#define MAXSIZE 100000

struct sockaddr_in connect_to_server(void)
{
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd == -1)
	{
		perror("socket");
		log_perror("socket\n");
		exit(EXIT_FAILURE);
	}

	int optval1 = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval1, sizeof(int)) == -1)
	{
		perror("setsockopt");
		log_perror("setsockopt\n");
		exit(EXIT_FAILURE);
	}

	int optval2 = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &optval2, sizeof(int)) == -1)
	{
		perror("setsockopt");
		log_perror("setsockopt\n");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(MY_SOCK_PATH);
	server_addr.sin_addr.s_addr = INADDR_BROADCAST;

	char message[] = "Hello from the client";

	if (sendto(sock_fd, &message, sizeof(message), 0,
			   (struct sockaddr *)&server_addr,
			   sizeof(struct sockaddr_in)) == -1)
	{
		perror("sendto");
		log_perror("sendto\n");
		exit(EXIT_FAILURE);
	}

	char buf[MAXSIZE];

	socklen_t addrlen = sizeof(buf);

	int n = recvfrom(sock_fd, buf, sizeof(buf), 0,
					 (struct sockaddr *)&server_addr,
					 &addrlen);

	char *ip = inet_ntoa(server_addr.sin_addr);
	unsigned int port = server_addr.sin_port;
	buf[n] = '\0';
	printf("broadcast message: %s\nfrom server: %s:%d\n", buf, ip, port);
	close(sock_fd);

	return server_addr;
}

struct sockaddr_in connect_to_client(void)
{
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd == -1)
	{
		perror("socket");
		log_perror("socket\n");
		exit(EXIT_FAILURE);
	}

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(MY_SOCK_PATH);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock_fd, (struct sockaddr *)&server_addr,
			 sizeof(struct sockaddr_in)) == -1)
	{
		perror("bind");
		log_perror("bind\n");
		exit(EXIT_FAILURE);
	}

	char buf[MAXSIZE];
	socklen_t addrlen = sizeof(buf);
	struct sockaddr_in client_addr;
	int n = recvfrom(sock_fd, buf, sizeof(buf), 0,
					 (struct sockaddr *)&client_addr,
					 &addrlen);
	buf[n] = '\0';

	char *ip = inet_ntoa(client_addr.sin_addr);
	unsigned int port = client_addr.sin_port;
	printf("broadcast message: %s\nfrom client : %s:%d\n", buf, ip, port);

	char message[] = "Hello from the server";

	if (sendto(sock_fd, &message, sizeof(message), 0,
			   (struct sockaddr *)&client_addr,
			   sizeof(client_addr)) == -1)
	{
		perror("sendto");
		log_perror("sendto\n");
		exit(EXIT_FAILURE);
	}

	close(sock_fd);

	return server_addr;
}
