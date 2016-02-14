#ifndef COMMON_H
#define COMMON_H

#include <assert.h>
#include <stdbool.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>
#include <unistd.h>

// at least 128 bytes for SSL!!!
#define ERR_BUF_LEN	    256

#define BUDDY_PORT  "11437"

#define MAX_INCOMING_CONNECTIONS    64
#define MAX_EVENTS  (MAX_INCOMING_CONNECTIONS + 10)

#define msyslog(prior, fmt, args...) syslog(prior, "[%s %s] " fmt, __FILE__, __func__, ##args) 
#define log_errno(e) _log_errno(__FILE__, __func__, e);
void _log_errno(const char *file, const char *func, int e);

#endif // COMMON_H
