#include "general.h"
#include "comm.h"
#include "datachunks.h"
#include <sys/epoll.h>
#include <stdio.h>

#ifndef NDEBUG
    #include "test.h"
#endif /* !NDEBUG */

void app_shutdown(int sockfd, struct addrinfo *ai, struct sfd_dcl_storage *sfd_dcl);
void process_epoll_socket_error(struct epoll_event *event, int *main_sockfd, int *state);

int main(int argc, char *argv[])
{
    struct addrinfo *ai = NULL;
    char ip[INET_ADDRSTRLEN];

    /* Main socket for out client connection */
    int sockfd = -1;
    /* Main epoll queue fd */
    int efd = -1;
    struct epoll_event events[MAX_EVENTS];
    struct sfd_dcl_storage *sfd_dcl = NULL;
    int n, i;

    /*----- APP INIT SECTION */
    openlog("ydd-client", LOG_PID, LOG_USER);
    /*----- APP INIT SECTION END */
    
    get_server_addrinfo("192.168.1.78", "11437", false, &ai);
    get_ip_string(ai, ip);
    sockfd = open_connection(ai);
    if(sockfd == -1) {
	msyslog(LOG_ERR, "Failed open_connection");
	app_shutdown(sockfd, ai, sfd_dcl);
	return -1;
    }

    if(make_socket_non_blocking(sockfd) == -1) {
	syslog(LOG_ERR, "Failed to make socket non-blocking");
	app_shutdown(sockfd, ai, sfd_dcl);
	return -1;
    }

    efd = epoll_create1(0);
    if(efd == -1) {
	msyslog(LOG_ERR, "Failed epoll_create1(0)");
	app_shutdown(sockfd, ai, sfd_dcl);
	return -1;
    }

    struct epoll_event event;
    event.data.fd = sockfd;
    event.events = EPOLLIN | EPOLLOUT | EPOLLET;
    n = epoll_ctl (efd, EPOLL_CTL_ADD, sockfd, &event);
    if(n == -1) {
	msyslog(LOG_ERR, "Failed to EPOLL_CTL_ADD for sockfd %d", sockfd);
	app_shutdown(sockfd, ai, sfd_dcl);
	return -1;
    }

    sfd_dcl = sfd_dcl_create(SFD_DCL_STORAGE_SIZE);
    if(sfd_dcl == NULL) {
	msyslog(LOG_ERR, "Failed to create the main SFD-DCL storage");
	app_shutdown(sockfd, ai, sfd_dcl);
    }

#define MAIN_FSM_CYCLING	0
#define MAIN_FSM_SOCKET_ERROR	-1
    int state = MAIN_FSM_CYCLING;

    while(state == MAIN_FSM_CYCLING) {
	n = epoll_wait(efd, events, MAX_EVENTS, -1);
	for(i = 0; (i < n) && (state == MAIN_FSM_CYCLING); i++) {
	    if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) {
		process_epoll_socket_error(&(events[i]), &sockfd, &state);
		continue;
	    }

	    else if(events[i].events & EPOLLIN) {
	    }

	    else if(events[i].events & EPOLLOUT) {
	    }
	}
    }

    /* ---- APP SHUTDOWN SECTION */
    app_shutdown(sockfd, ai, sfd_dcl);

    return 0;
}

void app_shutdown(int sockfd, struct addrinfo *ai, struct sfd_dcl_storage *sfd_dcl)
{
    sfd_dcl_empty_and_kill(sfd_dcl);
    close_socket(sockfd);
    freeaddrinfo(ai);
    closelog();
}

void process_epoll_socket_error(struct epoll_event *event, int *main_sockfd, int *state)
{
    if((event == NULL) || (state == NULL) || main_sockfd == NULL)
	return;
    if(event->data.fd == *main_sockfd) {
	*state = MAIN_FSM_SOCKET_ERROR;
	msyslog(LOG_CRIT, "Got an epoll-error on the main socket (fd = %d). Can't proceed, will exit", *main_sockfd);
	*main_sockfd = -1;
    }
    else {
	msyslog(LOG_ERR, "epoll error on socket %d", event->data.fd);
    }
    close(event->data.fd);
}
