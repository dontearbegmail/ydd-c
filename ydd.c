#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <syslog.h>

#include <sys/epoll.h>

#include "comm.h"
#include "httpchunks.h"
#include "ydapi_comm.h"

#include <errno.h>


#define MAXEVENTS  64

void app_shutdown(int sockfd, struct addrinfo *ai);

int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo *ai;
    char ip[INET_ADDRSTRLEN];

    //----- APP INIT SECTION
    openlog(NULL, LOG_PID, LOG_USER);
    //----- APP INIT SECTION END
    
    get_server_addrinfo("perenesem.ru", "80", false, &ai);
    get_ip_string(ai, ip);
    sockfd = open_connection(ai);
    if(sockfd == -1) {
	syslog(LOG_ERR, "Failed open_connection");
	app_shutdown(sockfd, ai);
	return -1;
    }
    
    size_t sz = 4194304;
    char msg[sz];
    memset(msg, 'a', sz);
    msg[((size_t) sz - 1)] = 0;
    /*
    char *request = "GET / HTTP/1.1\r\nHost: lenta.ru\r\n\r\n";
    if(send_string_to_socket(sockfd, request)) {
	syslog(LOG_ERR, "Just a part of the request has been sent");
    }
    else {
	read_from_socket(sockfd);
    }*/

    if(make_socket_non_blocking(sockfd) == -1) {
	syslog(LOG_ERR, "Failed to make socket non-blocking");
	app_shutdown(sockfd, ai);
	return -1;
    }

    size_t n = send(sockfd, msg, sz, 0);
    if(n == -1)
    {
	int er = errno;
	if(er == EAGAIN || er == EWOULDBLOCK) {
	    n = 0;
	}
    }
/*
    int efd, s;
    struct epoll_event event;
    struct epoll_event events[MAXEVENTS];

    efd = epoll_create1(0);
    if(efd == -1) {
	syslog(LOG_ERR, "Failed epoll_create1");
	app_shutdown(sockfd, ai);
	return -1;
    }

    event.data.fd = sockfd;
    event.events = EPOLLIN;
    s = epoll_ctl(efd, EPOLL_CTL_ADD, sockfd, &event);
    if(s == -1) {
	syslog(LOG_ERR, "Failed to EPOLL_CTL_ADD of read-write events on socket");
	app_shutdown(sockfd, ai);
	return -1;
    }
*/


    // ---- APP SHUTDOWN SECTION
    app_shutdown(sockfd, ai);

    return 0;
}

void app_shutdown(int sockfd, struct addrinfo *ai)
{
    close_socket(sockfd);
    freeaddrinfo(ai);
    closelog();
}
