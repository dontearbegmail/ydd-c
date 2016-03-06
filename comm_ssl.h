#ifndef COMM_SSL_H
#define COMM_SSL_H

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <netdb.h>

int init_ssl(SSL_CTX **out_ctx, SSL **out_ssl);
void shutdown_ssl(SSL_CTX *ctx, SSL *ssl);
int open_ssl_connection(struct addrinfo *ai, SSL *ssl);
int close_ssl_connection(int sockfd, SSL *ssl);

#endif /* COMM_SSL_H */
