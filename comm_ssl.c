#include "general.h"
#include "comm_ssl.h"

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
    SSL_load_error_strings(); /* is void */
    SSL_library_init(); /* always returns 1 */

    *out_ctx = SSL_CTX_new(SSLv23_method()); 
    if(NULL == *out_ctx) {
	log_ssl_errors();
	return -1;
    }
    *out_ssl = SSL_new(*out_ctx);
    if(NULL == *out_ssl) {
	SSL_CTX_free(*out_ctx); /* void */
	*out_ctx = NULL;
	log_ssl_errors();
	return -1;
    }

    return 0;
}

void shutdown_ssl(SSL_CTX *ctx, SSL *ssl)
{
    if(ssl != NULL)
	SSL_free(ssl); /* void */
    if(ctx != NULL)
	SSL_CTX_free(ctx); /* void */
    ERR_free_strings(); /* void */
}

int open_ssl_connection(struct addrinfo *ai, SSL *ssl)
{
    if(ai == NULL)
	return -1;
    if(ssl == NULL)
	return -1;

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

    if(ssl != NULL)
	SSL_shutdown(ssl);
    if(sockfd != -1)
	retval = close(sockfd);
    return retval;
}


