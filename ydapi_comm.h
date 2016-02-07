#ifndef YDAPI_COMM_H
#define YDAPI_COMM_H

#define YDAPI_SANDBOX

#ifndef YDAPI_SANDBOX
	#define YDAPI_SERVADDR "api.direct.yandex.ru"
#else
	#define YDAPI_SERVADDR "api-sandbox.direct.yandex.ru"
#endif

#define YDAPI_SERVPORT "443" 
#define YDAPI_HTTPH_POST_LINE "POST /v4/json/ HTTP/1.1\r\n"
#define YDAPI_HTTPH_CONTENT_TYPE_LINE "Content-Type: application/json; charset=utf-8\r\n"
#define YDAPI_HTTPH_CONTENT_LENGTH_LINE "Content-Length: "
#define YDAPI_HTTPH_HOST_LINE "Host: "

#include "comm.h"

struct ydapi_httph_lines {
	char *post;
	size_t post_len;

	char *host;
	size_t host_len;

	char *content_length;
	size_t content_length_len;
	
	char *content_type;
	size_t content_type_len;

	// This variable holds size of program-constant HTTP header part IN BYTES:
	// (not including terminating 0 char!!!)
	//
	// All HTTP header lines are constant during execution time, except 
	// "Content-Length: " value. So we calculate the HTTP header size
	// just once and put it in this variable. 
	//
	// To get the complete HTTP header bytes count it's necessary to add:
	// - strlen(<content length>) - should be added to "Content-Length: " line
	// - sizeof("\r\n") - should be added to "Content-Length: " line 
	// ATTENTION!!! It's assumed that all HTTP header contents consists of 
	// single-byte characters, i.e. strlen("string") == sizeof("string")
	// NOT FOR USE WITH wchar_t (multibyte strings)!!!
	
	size_t constant_len; 

	char *constant;
};

struct ydapi_comm {
    struct ydapi_httph_lines *httphl;
    struct addrinfo *srvinfo;
    char srvip[INET_ADDRSTRLEN];
};

struct ydapi_comm * ydapi_comm_init();
void ydapi_comm_shutdown(struct ydapi_comm *yc);

struct ydapi_httph_lines * ydapi_init_httph_lines();
void ydapi_empty_and_kill_httph_lines(
		struct ydapi_httph_lines *ydapi_httphl);
int ydapi_generate_http_request(
		struct ydapi_httph_lines *ydapi_httphl,
		char *json_query,
	   	char **out_http_request);

#endif // YDAPI_COMM_H
