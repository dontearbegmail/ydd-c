#include "comm.h"

#include <unistd.h>

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <openssl/conf.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "httpchunks.h"
#include "common.h"

int send_string_to_socket(int sockfd, char *str)
{
    size_t len, bytes_sent, remaining_len, total_bytes_sent;
    len	= strlen(str);
    remaining_len = len;
    total_bytes_sent = 0;
    do {
	bytes_sent = send(sockfd, (str + total_bytes_sent), remaining_len, 0);
	if(bytes_sent > 0) {
	    total_bytes_sent += bytes_sent;
	    remaining_len = len - total_bytes_sent;
	}
	else {
	    break;
	}
    } while(total_bytes_sent < len);
    return (bytes_sent == -1) ? -1 : (total_bytes_sent != len);
}

int read_from_socket(int sockfd)
{
    int size_recv;
    char chunk[HTTP_CHUNK_SIZE];
    //while(1)
    //{
    memset(chunk, 0, HTTP_CHUNK_SIZE);
    if((size_recv = recv(sockfd,  chunk, HTTP_CHUNK_SIZE, 0)) <= 0) {
	//		break;
    }
    else {
	printf("%s", chunk);
    }
    //	}

    return 0;
}

void get_ip_string(struct addrinfo *ai, char *inet_addrstr)
{
    struct sockaddr_in *sai = (struct sockaddr_in *)ai->ai_addr; 
    inet_ntop(AF_INET, &(sai->sin_addr), inet_addrstr, INET_ADDRSTRLEN);
}


int get_server_addrinfo(char *host, char *port, bool is_listening, struct addrinfo **out_server_addrinfo)
{
    struct addrinfo hints, *result;
    int retval = -1;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if(is_listening)
	hints.ai_flags = AI_PASSIVE;

    int s = getaddrinfo(host, port, &hints, &result);
    if (s != 0) {
	msyslog(LOG_ERR, "getaddrinfo: %s\n", gai_strerror(s));
    }
    else {
	*out_server_addrinfo = result;
	retval = 0;
    }

    return retval;
}

void log_ssl_errors(void)
{
    unsigned long e = ERR_get_error();

    char buf[ERR_BUF_LEN];
    while(e != 0UL) {
	e = ERR_get_error();
	ERR_error_string_n(e, buf, ERR_BUF_LEN);
	msyslog(LOG_ERR, "%s", buf);
    }
}

int init_ssl(SSL_CTX **out_ctx, SSL **out_ssl)
{
    SSL_load_error_strings(); // is void
    SSL_library_init(); // always returns 1

    *out_ctx = SSL_CTX_new(SSLv23_method()); 
    if(NULL == *out_ctx) {
	log_ssl_errors();
	return -1;
    }
    *out_ssl = SSL_new(*out_ctx);
    if(NULL == *out_ssl) {
	SSL_CTX_free(*out_ctx); // void
	*out_ctx = NULL;
	log_ssl_errors();
	return -1;
    }

    return 0;
}

void shutdown_ssl(SSL_CTX *ctx, SSL *ssl)
{
    assert(NULL != ctx);
    assert(NULL != ssl);
    SSL_free(ssl); // void
    SSL_CTX_free(ctx); // void
    ERR_free_strings(); // void
}

int open_ssl_connection(struct addrinfo *ai, SSL *ssl)
{
    assert(NULL != ai);
    assert(NULL != ssl);

    int e;
    int sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if(sockfd == -1) {
	e = errno;
	log_errno(e);
	return -1;
    }

    if(connect(sockfd, ai->ai_addr, ai->ai_addrlen) != 0) {
	e = errno;
	log_errno(e);
	return -1;
    }

    if(SSL_set_fd(ssl, sockfd) != 1) {
	log_ssl_errors();
	return -1;
    }

    e = SSL_connect(ssl);
    if (e != 1) {
	int f;
	ERR_print_errors_fp(stdout);
	switch(SSL_get_error(ssl, e))
	{
	    case SSL_ERROR_NONE:
		return SSL_ERROR_NONE;
		break;
	    case SSL_ERROR_ZERO_RETURN:
		return SSL_ERROR_ZERO_RETURN;
		break;
	    case SSL_ERROR_WANT_READ:
		return SSL_ERROR_WANT_READ;
		break;
	    case SSL_ERROR_WANT_WRITE:
		return SSL_ERROR_WANT_WRITE;
		break;
	    case SSL_ERROR_WANT_CONNECT:
		return SSL_ERROR_WANT_CONNECT;
		break;
	    case SSL_ERROR_WANT_ACCEPT:
		return SSL_ERROR_WANT_ACCEPT;
		break;
	    case SSL_ERROR_WANT_X509_LOOKUP:
		return SSL_ERROR_WANT_X509_LOOKUP;
		break;
		/*case SSL_ERROR_WANT_ASYNC:
		  return 0;
		  break;*/

	    case SSL_ERROR_SYSCALL:
		f = ERR_get_error();
		return SSL_ERROR_SYSCALL;
		break;

	    case SSL_ERROR_SSL:
		return SSL_ERROR_SSL;
		break;

	    default:
		return 0;
		break;
	}
	return -1;
    }

    return sockfd;
}

int close_ssl_connection(int sockfd, SSL *ssl)
{
    int retval = -1;
    assert(NULL != ssl);

    SSL_shutdown(ssl);
    if(sockfd != -1)
	retval = close(sockfd);
    return retval;
}

int open_connection(struct addrinfo *ai)
{
    assert(NULL != ai);

    int e;
    int sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if(sockfd == -1) {
	e = errno;
	log_errno(e);
	return -1;
    }

    if(connect(sockfd, ai->ai_addr, ai->ai_addrlen) != 0) {
	e = errno;
	log_errno(e);
	return -1;
    }
    return sockfd;
}

int close_connection(int sockfd)
{
    int retval = -1;

    if(sockfd != -1)
	retval = close(sockfd);
    return retval;
}

int make_socket_non_blocking(int sockfd)
{
    int flags, s;

    flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
	msyslog(LOG_ERR, "make_socket_non_blocking: couldn't get flags for socket");
	return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl(sockfd, F_SETFL, flags);
    if (s == -1) {
	msyslog(LOG_ERR, "make_socket_non_blocking: failed to set O_NONBLOCK");
	return -1;
    }

    return 0;
}

int create_and_bind_socket(char *port)
{
    assert(port != NULL);

    struct addrinfo *ai;
    char ip[INET_ADDRSTRLEN];
    int s, sockfd;

    get_server_addrinfo(NULL, port, true, &ai);
    get_ip_string(ai, ip);
    msyslog(LOG_INFO, "Will try to bind to %s:%s", ip, port);

    sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if(sockfd == -1) {
	s = errno;
	log_errno(s);
	freeaddrinfo(ai);
	return -1;
    }

    s = bind(sockfd, ai->ai_addr, ai->ai_addrlen);
    if(s != 0) {
	s = errno;
	log_errno(s);
	freeaddrinfo(ai);
	return -1;
    }

    freeaddrinfo(ai);
    return sockfd;
}
