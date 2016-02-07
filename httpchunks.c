#include <stdlib.h>
#include <string.h>
#include "httpchunks.h"

struct http_chunks_list * hcl_create()
{
    struct http_chunks_list *new_hcl = malloc(sizeof(struct http_chunks_list));
    new_hcl->first = NULL;
    new_hcl->last = NULL;
    new_hcl->length = 0;

    return new_hcl;
}

void hcl_empty_and_kill(struct http_chunks_list *hcl)
{
    struct http_chunk *hc, *next_hc;
    if(hcl == NULL)
	return;

    next_hc = hcl->first;
    while(next_hc != NULL) {
	hc = next_hc;
	next_hc = hc->next;
	free(hc);
    }
    free(hcl);
}

int hcl_add_chunk(struct http_chunks_list *hcl, char *chunk, size_t size_below_default)
{
    struct http_chunk *hc = malloc(sizeof(struct http_chunk));
    size_t len = (size_below_default == 0) ? HTTP_CHUNK_SIZE : size_below_default;

    if(len > HTTP_CHUNK_SIZE)
	return -1;

    memcpy(hc->chunk, chunk, len);
    hc->actual_size = len;
    hc->next = NULL;
    if(hcl->first == NULL) 
	hcl->first = hc;
    else 
	hcl->last->next = hc;
    hcl->last = hc;
    hcl->length++;

    return 0;
}
