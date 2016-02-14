#include <stdlib.h>
#include <string.h>
#include "datachunks.h"

struct data_chunks_list * dcl_create()
{
    struct data_chunks_list *new_dcl = malloc(sizeof(struct data_chunks_list));
    new_dcl->first = NULL;
    new_dcl->last = NULL;
    new_dcl->length = 0;

    return new_dcl;
}

void dcl_empty_and_kill(struct data_chunks_list *dcl)
{
    struct data_chunk *dc, *next_dc;
    if(dcl == NULL)
	return;

    next_dc = dcl->first;
    while(next_dc != NULL) {
	dc = next_dc;
	next_dc = dc->next;
	free(dc);
    }
    free(dcl);
}

int dcl_add_chunk(struct data_chunks_list *dcl, char *chunk, size_t size_below_default)
{
    struct data_chunk *dc = malloc(sizeof(struct data_chunk));
    size_t len = (size_below_default == 0) ? DATA_CHUNK_SIZE : size_below_default;

    if(len > DATA_CHUNK_SIZE)
	return -1;

    memcpy(dc->chunk, chunk, len);
    dc->actual_size = len;
    dc->next = NULL;
    if(dcl->first == NULL) 
	dcl->first = dc;
    else 
	dcl->last->next = dc;
    dcl->last = dc;
    dcl->length++;

    return 0;
}
