#ifndef FSM_SOCKET_H
#define FSM_SOCKET_H

#include "general.h"
#include "socket_data.h"

enum fsms_states {
    fsms_q_none,
    fsms_q_create = 1,
    fsms_q_make_non_blocking,
    fsms_q_epoll,
    fsms_q_connect,
    fsms_q_read,
    fsms_q_read_epollin_pending,
    fsms_q_write,
    fsms_q_write_epollout_pending,
    fsms_q_ssl_write,
    fsms_q_ssl_write_epollin_pending,
    fsms_q_ssl_write_epollout_pending,
    fsms_q_ssl_read,
    fsms_q_ssl_read_epollout_pending,
    fsms_q_ssl_read_epollin_pending,
    fsms_q_shutdown
};

enum fsms_signals {
    fsms_s_none = 0,
    fsms_s_loopback = 1,
    fsms_s_noerr,
    fsms_s_err,
    fsms_s_einprogress,
    fsms_s_epollin,
    fsms_s_epollout,
    fsms_s_ssl_error_want_read,
    fsms_s_ssl_error_want_write,
    fsms_s_empty
};
#define FSMS_NUM_SIGNALS   10 

typedef enum fsms_signals (*fsms_transition_callback)(struct sfd_sd_storage *sfd_sd);

struct fsms_transition {
    enum fsms_states new_state;
    fsms_transition_callback worker;
};

struct fsm_socket {
    enum fsms_states state;
    struct addrinfo *ai;
};

enum fsms_signals fsms_w_create(struct sfd_sd_storage *sfd_sd);

#endif // FSM_SOCKET_H
