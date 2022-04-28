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
	server_addr = connect_to_client();

	if (select_protocol() == 1)
	{
		printf("TCP selected\n");
		log_info("TCP selected\n");
		tcp_server(&server_addr);
	}

	else
	{
		printf("UDP selected\n");
		log_info("UDP selected\n");
		udp_server(&server_addr);
	}
}
