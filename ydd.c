#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <syslog.h>

#include "comm.h"
#include "httpchunks.h"
#include "ydapi_comm.h"
#include "ydd_libevent.h"

int main(int argc, char *argv[])
{
    int sockfd;
    SSL_CTX *ctx;
    SSL *ssl;
    struct ydapi_comm *yc;

    //----- APP INIT SECTION
    openlog(NULL, LOG_PID, LOG_USER);
    yle_setup();
    init_ssl(&ctx, &ssl);
    yc = ydapi_comm_init();
    //----- APP INIT SECTION END

    time_t mytime = time(NULL);
    char smt[20];
    strftime(smt, 20, "%Y-%m-%d %H:%M:%S", localtime(&mytime));
    printf("%s is %s.\r\nRequest time: %s\r\n\r\n", YDAPI_SERVADDR, yc->srvip, smt);

    sockfd = open_ssl_connection(yc->srvinfo, ssl);

    // 15636a0007164adfbea78aea50f8c643
    // 1eb646af64a544a98fb66900e11ee7d6

    char json_request[] = "{\"method\":\"GetAvailableVersions\",\"locale\":\"ru\",\"token\":\"15636a0007164adfbea78aea50f8c643\"}";
    //char json_request[] = "{\"method\":\"GetVersion\",\"token\":\"15636a0007164adfbea78aea50f8c643\"}";
    //char json_request[] = "{\"method\":\"CreateNewWordstatReport\",\"token\":\"15636a0007164adfbea78aea50f8c643\"}";

    char *request;
    ydapi_generate_http_request(yc->httphl, json_request, &request);
    printf("%s\r\n", request);

    if(SSL_write(ssl, request, strlen(request)) <= 0) {
	//ERR_print_errors_fp(stderr);
	return -1;
    }
    free(request);

    char buf[HTTP_CHUNK_SIZE];
    if(SSL_read(ssl, buf, HTTP_CHUNK_SIZE) <= 0) {
	//ERR_print_errors_fp(stderr);
	return -1;
    }

    printf("%s\n", buf);
    close_ssl_connection(sockfd, ssl);

    // ---- APP SHUTDOWN SECTION
    ydapi_comm_shutdown(yc);
    shutdown_ssl(ctx, ssl);
    closelog();

    return 0;
}


