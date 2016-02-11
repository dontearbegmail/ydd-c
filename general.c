#include "general.h"

void log_errno(int e)
{
    char buf[ERR_BUF_LEN];
    strerror_r(e, buf, ERR_BUF_LEN);
    syslog(LOG_ERR, buf);
}


