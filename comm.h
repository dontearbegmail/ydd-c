#ifndef COMM_H
#define COMM_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <stdbool.h>

int get_server_addrinfo(char *host, char *port, bool is_listening, struct addrinfo **out_server_addrinfo);
void get_ip_string(struct addrinfo *ai, char *inet_addrstr);

int init_ssl(SSL_CTX **out_ctx, SSL **out_ssl);
void shutdown_ssl(SSL_CTX *ctx, SSL *ssl);
int open_ssl_connection(struct addrinfo *ai, SSL *ssl);
int close_ssl_connection(int sockfd, SSL *ssl);

int open_connection(struct addrinfo *ai);
int close_connection(int sockfd);
int send_string_to_socket(int sockfd, char *str);
int read_from_socket(int sockfd);
int make_socket_non_blocking(int sockfd);

void log_errno(int e);

#endif // COMM_H
