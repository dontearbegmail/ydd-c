#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "ydapi_comm.h"

struct ydapi_httph_lines * ydapi_init_httph_lines()
{
    struct ydapi_httph_lines *ydapi_httphl = malloc(
	    sizeof(struct ydapi_httph_lines));
    size_t l, tl;

    l = strlen(YDAPI_HTTPH_POST_LINE);
    ydapi_httphl->post = malloc(sizeof(char) * (l + 1));
    strcpy(ydapi_httphl->post, YDAPI_HTTPH_POST_LINE);
    ydapi_httphl->post_len = l;

    tl = strlen(YDAPI_HTTPH_HOST_LINE);
    l = tl + strlen(YDAPI_SERVADDR) + 2; // add \r\n
    ydapi_httphl->host = malloc(sizeof(char) * (l + 1));
    strcpy(ydapi_httphl->host, YDAPI_HTTPH_HOST_LINE);
    strcat(ydapi_httphl->host, YDAPI_SERVADDR);
    strcat(ydapi_httphl->host, "\r\n");
    ydapi_httphl->host_len = l;

    l = strlen(YDAPI_HTTPH_CONTENT_LENGTH_LINE);
    ydapi_httphl->content_length = malloc(sizeof(char) * (l + 1));
    strcpy(ydapi_httphl->content_length, YDAPI_HTTPH_CONTENT_LENGTH_LINE);
    ydapi_httphl->content_length_len = l;

    l = strlen(YDAPI_HTTPH_CONTENT_TYPE_LINE);
    ydapi_httphl->content_type = malloc(sizeof(char) * (l + 1));
    strcpy(ydapi_httphl->content_type, YDAPI_HTTPH_CONTENT_TYPE_LINE);
    ydapi_httphl->content_type_len = l;

    // calculate constant_len
    size_t rl = ydapi_httphl->post_len + ydapi_httphl->host_len + 
	ydapi_httphl->content_length_len + ydapi_httphl->content_type_len;
    // 
    // !!! constant_len doesn't include \r\n for "Content-Length"
    // rl += 2;
    //
    // !!! constant_len doesn't include terminating 0 char!!!
    ydapi_httphl->constant_len = rl;

    ydapi_httphl->constant = malloc(sizeof(char) * (rl + 1));
    strcpy(ydapi_httphl->constant, ydapi_httphl->post);
    strcat(ydapi_httphl->constant, ydapi_httphl->host);
    strcat(ydapi_httphl->constant, ydapi_httphl->content_type);
    strcat(ydapi_httphl->constant, ydapi_httphl->content_length);

    return ydapi_httphl;
}

void ydapi_empty_and_kill_httph_lines(struct ydapi_httph_lines *ydapi_httphl)
{
    free(ydapi_httphl->post);
    free(ydapi_httphl->host);
    free(ydapi_httphl->content_length);
    free(ydapi_httphl->content_type);
    free(ydapi_httphl->constant);
    free(ydapi_httphl);
}

size_t __ydapi_calc_http_request_bufsize(
	struct ydapi_httph_lines *ydapi_httphl,
	char *json_query,
	char (*ref_string_content_length_rn)[])
{
    size_t bufsize;
    size_t cl; // content-length
    cl = strlen(json_query); // FIXME for multibyte!!!
    sprintf(*ref_string_content_length_rn, "%d\r\n", cl);

    bufsize = ydapi_httphl->constant_len;

    // this is total size of HTTP header in bytes assuming there are only 
    // singlebyte chars in there
    bufsize += strlen(*ref_string_content_length_rn);

    bufsize += 2;	// \r\n delimiter between HTTP header and body
    bufsize += cl;
    bufsize += 3;	// \r\n and \0 in the end of zero-terminated string

    return bufsize;
}

int ydapi_generate_http_request(
	struct ydapi_httph_lines *ydapi_httphl, 
	char *json_query, 
	char **out_http_request) 
{
    char scl[16];
    size_t rl = __ydapi_calc_http_request_bufsize(ydapi_httphl, json_query, &scl);

    *out_http_request = malloc(sizeof(char) * rl);
    strcpy(*out_http_request, ydapi_httphl->constant);
    strcat(*out_http_request, scl);
    strcat(*out_http_request, "\r\n");
    strcat(*out_http_request, json_query);
    strcat(*out_http_request, "\r\n");

    return 0;
}

struct ydapi_comm * ydapi_comm_init()
{
    struct ydapi_comm *yc = malloc(sizeof(struct ydapi_comm));
    yc->httphl = ydapi_init_httph_lines();
    get_server_addrinfo(YDAPI_SERVADDR, YDAPI_SERVPORT, false, &(yc->srvinfo)); 
    get_ip_string(yc->srvinfo, yc->srvip);

    return yc;
}

void ydapi_comm_shutdown(struct ydapi_comm *yc)
{
    ydapi_empty_and_kill_httph_lines(yc->httphl);
    freeaddrinfo(yc->srvinfo);

    free(yc);
}
