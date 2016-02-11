#include "general.h"
#include "comm.h"
#include "buddy.h"

#include <sys/epoll.h>

#define MAXEVENTS  64

void app_shutdown(int sockfd, struct addrinfo *ai);

int main(int argc, char *argv[])
{
    int sockfd = -1;
    struct addrinfo *ai;
    char ip[INET_ADDRSTRLEN];

    //----- APP INIT SECTION
    openlog("ydd", LOG_PID, LOG_USER);
    //----- APP INIT SECTION END

    get_server_addrinfo(NULL, BUDDY_PORT, true, &ai);
    get_ip_string(ai, ip);

    int e;
    sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if(sockfd == -1) {
	e = errno;
	log_errno(e);
	app_shutdown(sockfd, ai);
	return -1;
    }

    e = bind(sockfd, ai->ai_addr, ai->ai_addrlen);
    if(e != 0) {
	e = errno;
	log_errno(e);
	app_shutdown(sockfd, ai);
	return -1;
    }

    syslog(LOG_INFO, "Binded successfully");

    if(make_socket_non_blocking(sockfd) == -1) {
	syslog(LOG_ERR, "Failed to make socket non-blocking");
	app_shutdown(sockfd, ai);
	return -1;
    }

    e = listen(sockfd, 1);
    if(e == -1) {
	e = errno;
	log_errno(e);
	app_shutdown(sockfd, ai);
	return -1;
    }

    
    int efd, s;
    struct epoll_event event;
    //struct epoll_event events[MAXEVENTS];

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

    /*
    while(1) {
	int n, i;
	n = epoll_wait(efd, events, MAXEVENTS, -1);
    }*/

    // ---- APP SHUTDOWN SECTION
    app_shutdown(sockfd, ai);

    return 0;
}

void app_shutdown(int sockfd, struct addrinfo *ai)
{
    close_connection(sockfd);
    freeaddrinfo(ai);
    closelog();
}
