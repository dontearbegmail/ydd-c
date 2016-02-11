#ifndef COMM_H
#define COMM_H

#include <netdb.h>
#include "general.h"

int get_server_addrinfo(char *host, char *port, bool is_listening, struct addrinfo **out_server_addrinfo);
void get_ip_string(struct addrinfo *ai, char *inet_addrstr);

int open_connection(struct addrinfo *ai);
int close_connection(int sockfd);
int send_string_to_socket(int sockfd, char *str);
int read_from_socket(int sockfd);
int make_socket_non_blocking(int sockfd);

#endif // COMM_H
