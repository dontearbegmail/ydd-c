#ifndef COMM_H
#define COMM_H

#include <netdb.h>
#include "general.h"
#include "datachunks.h"

int get_server_addrinfo(char *host, char *port, bool is_listening, struct addrinfo **out_server_addrinfo);
void get_ip_string(struct addrinfo *ai, char *inet_addrstr);

int open_connection(struct addrinfo *ai);
int close_socket(int sockfd);
int send_string_to_socket(int sockfd, char *str);
int read_from_socket(int sockfd);
int make_socket_non_blocking(int sockfd);
int create_and_bind_socket(char *port, struct addrinfo **out_ai);
int init_epoll_and_listen(int sockfd, int op);
int accept_and_epoll(int listening_sfd, int efd, int op);

#define READ_S_ALL_DONE	    0
#define READ_S_GOT_EAGAIN   1
#define READ_S_GOT_ERROR    -1
#define READ_S_KEEP_READING 2
int read_form_socket_epollet(int sockfd, struct sfd_dcl_storage *sfd_dcl, size_t *ref_dcl_index);


#define SEND_S_ALL_DONE		0
#define SEND_S_W_SENT_SIZE	-2 /* if the whole size was sent but the send(..) function didn't return 0 */
#define SEND_S_W_SEND_FINISHED	-3 /* if the send(..) function returned 0, but ref_sent < size */
#define SEND_S_GOT_EAGAIN	1
#define SEND_S_GOT_ERROR	-1
#define SEND_S_KEEP_SENDING	2
int send_to_socket_epollet(int sockfd, char *data, size_t size, size_t *ref_sent);

#endif /* COMM_H */
