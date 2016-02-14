#include "general.h"
#include "comm.h"
#include "buddy.h"

#include <sys/epoll.h>

#define MAXEVENTS  64

void app_shutdown(int sockfd, struct addrinfo *ai);

int main(int argc, char *argv[])
{
    struct addrinfo *ai = NULL;
    int sockfd = -1;
    int efd = -1;
    struct epoll_event events[MAXEVENTS];
    int e;

    //----- APP INIT SECTION
    openlog("ydd", LOG_PID, LOG_USER);
    //----- APP INIT SECTION END

    sockfd = create_and_bind_socket(BUDDY_PORT);
    if(sockfd == -1)
	return -1;
    msyslog(LOG_INFO, "Binded successfully");

    efd = init_epoll_and_listen(sockfd, EPOLLIN);
    if(efd == -1) {
	msyslog(LOG_ERR, "Failed to non-block/listen the socket %d, or to init epoll", sockfd);
	app_shutdown(sockfd, ai);
	return -1;
    }

    while(1) {
	int n, i;
	n = epoll_wait(efd, events, MAXEVENTS, -1);
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
