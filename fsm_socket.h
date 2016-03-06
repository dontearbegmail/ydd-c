#ifndef FSM_SOCKET_H
#define FSM_SOCKET_H

#include "general.h"

#FSM_SOCKET_STATE_CONNECT		    1
#FSM_SOCKET_STATE_SSL_CONNECT		    2	
#FSM_SOCKET_STATE_SSL_READ		    3
#FSM_SOCKET_STATE_SSL_WRITE		    4

struct fsm_socket {
    int state;
};

#endif // FSM_SOCKET_H
