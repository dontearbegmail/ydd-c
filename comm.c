#include "comm.h"
#include "httpchunks.h"

#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>

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
	//printf("%s", chunk);
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

int close_socket(int sockfd)
{
    int retval = -1;

    if(sockfd != -1)
	retval = close(sockfd);

    if(retval == -1) {
	int e = errno;
	log_errno(e);
    }
    
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

    get_server_addrinfo("localhost", port, true, &ai);
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
	close_socket(sockfd);
	freeaddrinfo(ai);
	return -1;
    }

    freeaddrinfo(ai);
    return sockfd;
}

int init_epoll_and_listen(int sockfd, int op)
{
    int efd, s;
    struct epoll_event event;

    if(make_socket_non_blocking(sockfd) == -1) {
	msyslog(LOG_ERR, "Failed to make socket non-blocking");
	return -1;
    }

    s = listen(sockfd, 1);
    if(s == -1) {
	s = errno;
	log_errno(s);
	return -1;
    }
    
    efd = epoll_create1(0);
    if(efd == -1) {
	msyslog(LOG_ERR, "Failed epoll_create1");
	return -1;
    }

    event.data.fd = sockfd;
    event.events = op;
    s = epoll_ctl(efd, EPOLL_CTL_ADD, sockfd, &event);
    if(s == -1) {
	msyslog(LOG_ERR, "Failed to EPOLL_CTL_ADD of read-write events on socket");
	return -1;
    }

    return efd;
}
