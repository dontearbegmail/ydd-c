#include "general.h"
#include "comm.h"
#include <sys/epoll.h>

void app_shutdown(int sockfd, struct addrinfo *ai);

int main(int argc, char *argv[])
{
    struct addrinfo *ai = NULL;
    char ip[INET_ADDRSTRLEN];
    int sockfd = -1;
    int efd = -1;
    struct epoll_event events[MAX_EVENTS];
    int n, i;

    //----- APP INIT SECTION
    openlog("ydd", LOG_PID, LOG_USER);
    //----- APP INIT SECTION END

    sockfd = create_and_bind_socket(BUDDY_PORT, &ai);
    if(sockfd == -1)
	return -1;
    get_ip_string(ai, ip);
    msyslog(LOG_INFO, "Binded successfully to %s", ip);

    efd = init_epoll_and_listen(sockfd, EPOLLIN | EPOLLET);
    if(efd == -1) {
	msyslog(LOG_ERR, "Failed to non-block/listen the socket %d, or to init epoll", sockfd);
	app_shutdown(sockfd, ai);
	return -1;
    }
    msyslog(LOG_INFO, "Listening on %s:%s, non-blocking socket %d, epollfd %d", ip, BUDDY_PORT, sockfd, efd);

    while(1) {
	n = epoll_wait(efd, events, MAX_EVENTS, -1);
	for(i = 0; i < n; i++) {
	    if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))) {
		/* An error has occured on this fd, or the socket is not
		 *                  ready for reading (why were we notified then?) */
		msyslog(LOG_ERR, "epoll error on socket %d", events[i].data.fd);
		close(events[i].data.fd);
		continue;
	    }

	    else if(sockfd == events[i].data.fd) {
		msyslog(LOG_DEBUG, "Starting to accept incoming connections");
		while(1) {
		    int infd = accept_and_epoll(sockfd, efd, EPOLLET);
		    if(infd == 0) {
			msyslog(LOG_DEBUG, "Processed all incoming connections");
			break;
		    }
		    else if(infd == -1) {
			msyslog(LOG_ERR, "Failed to accept or epoll-queue an incoming connection. "
				"See log above for details");
			continue;
		    }
		    else {
			msyslog(LOG_INFO, "Successfully accepted an incoming connection on socket %d" 
				"and added it to epoll queue. See log above for details", infd);
		    }
		}

	    }

	    else {
		
	    }
	}
    }

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
