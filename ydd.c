#include "general.h"
#include "comm.h"
#include "datachunks.h"
#include <sys/epoll.h>
#include <stdio.h>

#ifndef NDEBUG
    #include "test.h"
#endif // !NDEBUG

void app_shutdown(int sockfd, struct addrinfo *ai, struct sfd_dcl_storage *sfd_dcl);
int sample_send(int sockfd, char *msg, size_t sz, size_t *ref_sent);

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
    openlog("ydd-client", LOG_PID, LOG_USER);
    //----- APP INIT SECTION END
    
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

    size_t sz = 1024 * 1024;
    char msg[sz];
    memset(msg, 'a', sz);
    msg[sz - 1] = 0;
    size_t sent = 0;

    while(1) {
	n = epoll_wait(efd, events, MAX_EVENTS, -1);
	for(i = 0; i < n; i++) {
	    /* An error has occured on this fd, or the socket is not
	     * ready for reading (why were we notified then?) */
	    if((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)) {
		msyslog(LOG_ERR, "epoll error on socket %d", events[i].data.fd);
		close(events[i].data.fd);
		continue;
	    }

	    /* An event on our main listening socket - say hello to incoming connection(s) */
	    /*else if(sockfd == events[i].data.fd) {
		//process_all_incoming_connections(sockfd, efd, sfd_dcl);
	    }*/

	    /*
	    else if(events[i].events & EPOLLIN) {
		size_t index, data_size;
		printf("Got a read on socket %d\n", events[i].data.fd);
		int read_state = read_form_socket_epollet(events[i].data.fd, sfd_dcl, &index);
		if(read_state == READ_S_ALL_DONE) {
		    msyslog(LOG_DEBUG, "Read all data on socket %d and will try to close the socket", 
			    events[i].data.fd);
		    close(events[i].data.fd);
		    char *d = dcl_get_data(sfd_dcl->dcls[index], &data_size);
		    printf("\n------\n%.*s\n-------\n", data_size, d);
		    if(d != NULL)
			free(d);
		    sfd_dcl_delete_index(sfd_dcl, index);
		}
		else if(read_state == READ_S_GOT_EAGAIN) {
		    printf("Got EAGAIN on socket %d. The data received by now: ", events[i].data.fd);
		    char *d = dcl_get_data(sfd_dcl->dcls[index], &data_size);
		    printf("%.*s\n", data_size, d);
		    if(d != NULL)
			free(d);
		}
	    }*/
	    else if((sockfd == events[i].data.fd) && (events[i].events & EPOLLOUT)) {
		int res = sample_send(sockfd, msg, sz, &sent);
		if(res == 0) {
		    printf("All sent in epoll cycle\n");
		}
		else if(res == -1) {
		    printf("Error in epoll cycle\n");
		}
		else {
		    printf("Sent by now %d bytes\n", sent);
		}
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

/* 0 = all sent
 * 1 = got epoll
 * -1 = error
 *
 * */
int sample_send(int sockfd, char *msg, size_t sz, size_t *ref_sent)
{
    bool done = false;
    bool got_epoll = false;
    bool got_error = false;
    ssize_t count;
    size_t sent = ref_sent == NULL ? 0 : * ref_sent;
    do {
	count = send(sockfd, msg + sent, sz - sent, 0);
	sleep(20);
	printf("Woke up\n");
	if(count == -1) {
	    int e = errno;
	    if(e == EAGAIN || e == EWOULDBLOCK) {
		got_epoll = true;
		printf("Got to epoll\n");
	    }
	    else {
		got_error = true;
		printf("Got error\n");
	    }
	}
	else if((count == 0) && (sent == sz)) {
	    done = true;
	    printf("Sent all data :( \n");
	}
	else {
	    sent += count;
	}
    } while(!done && !got_epoll && !got_error);
    if(ref_sent != NULL)
	*ref_sent = sent;

    if(got_error)
	return -1;
    if(got_epoll)
	return 1;
    return 0;
}
