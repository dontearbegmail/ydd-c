#ifndef COMMON_H
#define COMMON_H

#include <assert.h>
#include <stdbool.h>
#include <errno.h>
#include <syslog.h>
#include <string.h>

// at least 128 bytes for SSL!!!
#define ERR_BUF_LEN	    256


#define msyslog(prior, fmt, args...) syslog(prior, "[%s %s]" fmt, __FILE__, __func__, ##args) 
void log_errno(int e);

#endif // COMMON_H
