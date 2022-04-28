#ifndef TCP_UDP_H
#define TCP_UDP_H

int select_protocol(void);
void tcp_client(struct sockaddr_in *server_addr);
void udp_client(struct sockaddr_in *server_addr);
void tcp_server(struct sockaddr_in *server_addr);
void udp_server(struct sockaddr_in *server_addr);

#endif // TCP_UDP_H