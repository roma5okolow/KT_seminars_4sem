#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "../log/log.h"
#include "../broadcast/broadcast.h"
#include "../tcp-udp/tcp-udp.h"
#include "../pswd/pswd.h"

int main(int argc, char** argv)
{
	struct sockaddr_in server_addr;
	server_addr = connect_to_server();

	if (select_protocol() == 1)
	{
		printf("TCP selected\n");
		while (login_into_user() != 0)
			;
		tcp_client(&server_addr);
	}

	else
	{
		printf("UDP selected\n");
		while (login_into_user() != 0)
			;
		udp_client(&server_addr);
	}

}

