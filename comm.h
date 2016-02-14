#ifndef COMM_H
#define COMM_H

#include <netdb.h>
#include "general.h"

int get_server_addrinfo(char *host, char *port, bool is_listening, struct addrinfo **out_server_addrinfo);
void get_ip_string(struct addrinfo *ai, char *inet_addrstr);

int open_connection(struct addrinfo *ai);
int close_socket(int sockfd);
int send_string_to_socket(int sockfd, char *str);
int read_from_socket(int sockfd);
int make_socket_non_blocking(int sockfd);
int create_and_bind_socket(char *port, struct addrinfo **out_ai);
int init_epoll_and_listen(int sockfd, int op);
int accept_and_epoll(int listening_sfd, int efd, int op);

#endif // COMM_H
