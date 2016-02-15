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

void sfd_dcl_add(struct sfd_dcl_storage *sfd_dcl, int sockfd, char *chunk)
{
}

int put_to_sorted_array(int v, int *arr, size_t size, size_t *ref_count, size_t *ref_position)
{
    /*
    assert(size > 0);
    assert(ref_position != NULL);
    assert(ref_count != NULL);

    bool unique = true;
    size_t count = *ref_count;
    size_t pos = 0;
    bool found = false;

    if(count >= size) {
	msyslog(LOG_ERR, "Failed to add an item into a sorted array: count:%d >= size:%d", count, size);
	return -1;
    }

    if(count == 0) {
	arr[0] = v;
	pos = 0;
	count = 1;
    }
    else {
    }

    *ref_position = pos;
    *ref_count = count;*/
    return 0;
}

bool find_in_sorted_array(int v, int *arr, size_t size, size_t count, size_t *ref_position)
{
    bool found = false; // found == true means that value 'v' exists in array in arr[*ref_position]
    bool finished = false; // (finished == true) && (found == false) means that value 'v' doesn't exist in array,
			    // but *ref_position contains the position to put 'v'
    size_t start = 0;
    size_t end = count;
    size_t mid;
    /* If 0 <= pos <= count, it means that the array contents starting from arr[pos] should be shifted right,
     * and the new element 'v' should be posted to arr[pos] */
    size_t pos;

    /* Since our array and this function are intended to handle sockfds, and each new sockfd 
     * is typically bigger than all the previous ones, our bubble search is optimized 
     * considering this fact (when adding new)  */
    do {
	if(v > arr[end]) { // most searches will typically end here (when adding new)
	    finished = true;
	    pos = end + 1;
	}
	else if(v < arr[start]) {
	    finished = true;
	    pos = start;
	}
	else if(v == arr[start]) {
	    found = true;
	    pos = start + 1;
	}
	else if(v == arr[end]) {
	    found = true;
	    pos = end + 1;
	}
	else {
	    // now we have arr[start] < v < arr[end]
	    if(start >= end) {
		finished = true;
		pos = count;
	    }
	    else {
		mid = (start + end) / 2;
		if(v == arr[mid]) {
		    found = true;
		    pos = mid;
		}
		else if(v < arr[mid])
		    end = mid > 0 ? (mid - 1) : mid;
		else
		    start = mid + 1;
	    }
	}

	if(found)
	    finished = true;
    } while(!finished);
    return found;
}
