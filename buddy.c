#include "general.h"
#include "comm.h"
#include "datachunks.h"
#include <sys/epoll.h>
#include <stdio.h>

#ifndef NDEBUG
    #include "test.h"
#endif // !NDEBUG

void app_shutdown(int sockfd, struct addrinfo *ai, struct sfd_dcl_storage *sfd_dcl);
void process_all_incoming_connections(int sockfd, int efd, struct sfd_dcl_storage *sfd_dcl);

int main(int argc, char *argv[])
{
    struct addrinfo *ai = NULL;
    char ip[INET_ADDRSTRLEN];

    /* Main listening socket for accepting incoming connections */
    int sockfd = -1;
    /* Main epoll queue fd */
    int efd = -1;
    struct epoll_event events[MAX_EVENTS];
    struct sfd_dcl_storage *sfd_dcl = NULL;
    int n, i;

    //----- APP INIT SECTION
    openlog("ydd", LOG_PID, LOG_USER);
    //----- APP INIT SECTION END
    
#ifndef NDEBUG
    msyslog(LOG_DEBUG, "############# Running tests ");
    do_sfd_dcl_test(false);
    do_dcl_test(true);
    msyslog(LOG_DEBUG, "############# Finished tests ");
    return 0;
#endif //!NDEBUG

    sockfd = create_and_bind_socket(BUDDY_PORT, &ai);
    if(sockfd == -1)
	return -1;
    get_ip_string(ai, ip);
    msyslog(LOG_INFO, "Binded successfully to %s", ip);

    efd = init_epoll_and_listen(sockfd, EPOLLIN | EPOLLET);
    if(efd == -1) {
	msyslog(LOG_ERR, "Failed to non-block/listen the socket %d, or to init epoll", sockfd);
	app_shutdown(sockfd, ai, sfd_dcl);
	return -1;
    }
    msyslog(LOG_INFO, "Listening on %s:%s, non-blocking socket %d, epollfd %d", ip, BUDDY_PORT, sockfd, efd);

    sfd_dcl = sfd_dcl_create(SFD_DCL_STORAGE_SIZE);
    if(sfd_dcl == NULL) {
	msyslog(LOG_ERR, "Failed to create the main SFD-DCL storage");
	app_shutdown(sockfd, ai, sfd_dcl);
    }

    while(1) {
	n = epoll_wait(efd, events, MAX_EVENTS, -1);
	for(i = 0; i < n; i++) {
	    /* An error has occured on this fd, or the socket is not
	     * ready for reading (why were we notified then?) */
	    if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP) || (!(events[i].events & EPOLLIN))) {
		msyslog(LOG_ERR, "epoll error on socket %d", events[i].data.fd);
		close(events[i].data.fd);
		continue;
	    }

	    /* An event on our main listening socket - say hello to incoming connection(s) */
	    else if(sockfd == events[i].data.fd) {
		process_all_incoming_connections(sockfd, efd, sfd_dcl);
	    }

	    else {
		//int read_state = read_from_socket_epollet(events[i].data.fd, sfd_dcl);
	    }
	}
    }

    // ---- APP SHUTDOWN SECTION
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

void process_all_incoming_connections(int sockfd, int efd, struct sfd_dcl_storage *sfd_dcl)
{
    msyslog(LOG_DEBUG, "Starting to accept incoming connections");
    while(1) {
	int infd = accept_and_epoll(sockfd, efd, EPOLLET);
	if(infd == 0) {
	    msyslog(LOG_DEBUG, "Processed all incoming connections");
	    break;
	}
	else if(infd == -1) {
	    msyslog(LOG_ERR, "Failed to accept or epoll-queue an incoming connection. "
		    "Will stop processing incoming connections, some of them may be lost. "
		    "See log above for details");
	    break;
	}
	else {
	    msyslog(LOG_DEBUG, "Successfully accepted an incoming connection on socket %d " 
		    "and added it to epoll queue. See log above for details", infd);
	    int r = sfd_dcl_add(sfd_dcl, infd, NULL, 0);
	    if(r != 0) { 
		/* The saddest possible case: we're adding a new incoming connection socket file descriptor to our
		 * SFD-DCL storage, but it's value is already present there. How could it happen? The only explanation
		 * that comes into mind: we added this infd earlier, but the connection was closed, and we didn't
		 * notice that. So we have to clean DCL for that infd */
		if(r == 1) {
		    msyslog(LOG_WARNING, "sfd_dcl returned 1 when adding a new incoming connection "
			    "with socketfd = %d. Will empty the existing DCL", infd);
		    sfd_dcl_empty_dcl(sfd_dcl, infd);
		}
		else if(r == -1) {
		    msyslog(LOG_WARNING, "SFD-DCL storage limit reached while trying to add an incoming connection "
			    "with socketfd = %d. Will close the connection. TODO: add SFD-DCL storage cleaner", infd);
		    close(infd); // epoll removes infd automatically from its' watchlist
		}
		else if(r == -2) {
		    /* Iput data error?? (i.e. sfd_dcl == NULL) This should never happen, but still... */
		    msyslog(LOG_WARNING, "Lucky me! sfd_dcl_add returned -2 which means input data error. "
			    "Don't know what to do, so I'll continue hoping for the best and I'll pray for you :))");
		}
	    }
	    else {
		msyslog(LOG_DEBUG, "Successfully added the socketfd %d for the incoming connection "
			"to SFD-DCL storage.", infd);
	    }
	}
    }
}
