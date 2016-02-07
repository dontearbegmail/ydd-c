#include "ydd_libevent.h"
#include <syslog.h>

#include <event2/event.h>

static void yle_event_log_cb(int severity, const char *msg)
{
    const char *s;
    int p;
    switch(severity) {
	case EVENT_LOG_DEBUG: 
	    s = "debug";
	    p = LOG_DEBUG;
	    break;

	case EVENT_LOG_MSG:
	    s = "msg";
	    p = LOG_NOTICE;
	    break;

	case EVENT_LOG_WARN:
	    s = "warn";
	    p = LOG_WARNING;
	    break;

	case EVENT_LOG_ERR:
	    s = "err";
	    p = LOG_ERR;
	    break;

	default:
	    s = "?";
	    p = LOG_NOTICE;
	    break;
    }
    syslog(p, "[libevent %s] %s", s, msg);
}

void yle_setup(void)
{
    event_set_log_callback(yle_event_log_cb);
}
