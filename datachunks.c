#include <stdlib.h>
#include "datachunks.h"
#include "general.h"

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

struct sfd_dcl_storage *sfd_dcl_create(size_t size)
{
    assert(size > 0);
    struct sfd_dcl_storage *sfd_dcl = malloc(sizeof(struct sfd_dcl_storage));
    sfd_dcl->size = size;
    sfd_dcl->count = 0;
    sfd_dcl->socketfds = malloc(sizeof(int) * size);
    sfd_dcl->dcls = malloc(sizeof(struct sfd_dcl_storage *) * size);

    size_t i;
    for(i = 0; i < size; i++) {
	sfd_dcl->socketfds[i] = 0;
	sfd_dcl->dcls[i] = NULL;
    }

    return sfd_dcl;
}

void sfd_dcl_empty_and_kill(struct sfd_dcl_storage *sfd_dcl)
{
    if(sfd_dcl == NULL)
	return;

    size_t i;
    for(i = 0; i < sfd_dcl->count; i++) 
	dcl_empty_and_kill(sfd_dcl->dcls[i]);
    free(sfd_dcl->socketfds);
}

void sfd_dcl_add(int sfd_storage[], struct data_chunks_list *dcl_storage[], int size, int *count) {
    assert(count != NULL);
    assert(*count <= size);
}
