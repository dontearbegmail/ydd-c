#ifndef COMM_H
#define COMM_H

#include <netdb.h>
#include <openssl/ssl.h>

int get_server_addrinfo(char *host, char *port, struct addrinfo **out_server_addrinfo);
void get_ip_string(struct addrinfo *ai, char *inet_addrstr);

int init_ssl(SSL_CTX **out_ctx, SSL **out_ssl);
void shutdown_ssl(SSL_CTX *ctx, SSL *ssl);
int open_ssl_connection(struct addrinfo *ai, SSL *ssl);
int close_ssl_connection(int sockfd, SSL *ssl);

int send_string_to_socket(int sockfd, char *str);
int read_from_socket(int sockfd);

#endif // COMM_H
