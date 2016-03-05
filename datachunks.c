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

void dcl_empty(struct data_chunks_list *dcl)
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
    dcl->first = NULL;
    dcl->last = NULL;
    dcl->length = 0;
}

void dcl_empty_and_kill(struct data_chunks_list *dcl)
{
    if(dcl == NULL)
	return;
    dcl_empty(dcl);
    free(dcl);
}

int dcl_add_chunk(struct data_chunks_list *dcl, char *chunk, size_t size)
{
    if(dcl == NULL)
	return -1;
    if(chunk == NULL)
	return -1;
    
    size_t len = size;
    if((len > DATA_CHUNK_SIZE) || (len == 0))
	return -1;

    struct data_chunk *dc = malloc(sizeof(struct data_chunk));
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

char * dcl_get_data(struct data_chunks_list *dcl, size_t *ref_size)
{
    if(*ref_size != NULL)
	*ref_size = 0;
    if(dcl == NULL)
	return NULL;
    if(dcl->length == 0)
	return NULL;

    size_t total = 0;
    struct data_chunk *dc = dcl->first;
    while(dc != NULL) {
	total += dc->actual_size;
	dc = dc->next;
    }

    total++;
    char *data = malloc(total);
    int start = 0;
    dc = dcl->first;
    while(dc != NULL) {
	if((dc->chunk != NULL) && (dc->actual_size > 0))
	    memcpy(data + start, dc->chunk, dc->actual_size);
	start = start + dc->actual_size;
	dc = dc->next;
    }
    data[total - 1] = 0;
    if(ref_size != NULL)
	*ref_size = total;
    return data;
}

struct sfd_dcl_storage *sfd_dcl_create(size_t size)
{
    if(size == 0)
	return NULL;
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
    free(sfd_dcl->dcls);
    free(sfd_dcl->socketfds);
}

// returns -1 on array size reached, 1 if 'v' is not unique, 0 if everything's OK, -2 on input data errors
// if ref_index is not NULL, it will contain the index of sockfd and dcl
int sfd_dcl_add(struct sfd_dcl_storage *sfd_dcl, int sockfd, char *chunk, size_t size, size_t *ref_index)
{
    if(sfd_dcl == NULL)
	return -2;
    size_t pos, i, count = sfd_dcl->count;
    int r = put_to_sorted_array(sockfd, sfd_dcl->socketfds, sfd_dcl->size, &count, &pos, false);
    if(ref_index != NULL)
	*ref_index = pos;
    if(r == -1) {
	msyslog(LOG_ERR, "Can't process the new socket since sfd_dcl limit reached: size = %d, count = %d", 
		sfd_dcl->size, sfd_dcl->count);
    }
    else {
	if(r == 0) { // it means sockfd is a new, so sfd_dcl->socketfds were shifted right from 'pos' to 'count'
		    // and now we have to do the same with sfd_dcl->dcls
	    if(pos < sfd_dcl->count) {
		for(i = sfd_dcl->count; i > pos; i--) 
		    sfd_dcl->dcls[i] = sfd_dcl->dcls[i - 1];
	    }
	    sfd_dcl->dcls[pos] = dcl_create();
	    sfd_dcl->count = count;
	}
	dcl_add_chunk(sfd_dcl->dcls[pos], chunk, size);
    }

    return r;
}

bool sfd_dcl_delete_index(struct sfd_dcl_storage *sfd_dcl, size_t index) 
{
    if(sfd_dcl == NULL)
	return false;
    if(index >= sfd_dcl->count)
	return false;
    dcl_empty_and_kill(sfd_dcl->dcls[index]);
    size_t pos = index + 1;
    while(pos < sfd_dcl->count) {
	sfd_dcl->socketfds[pos - 1] = sfd_dcl->socketfds[pos];
	sfd_dcl->dcls[pos - 1] = sfd_dcl->dcls[pos];
	pos++;
    }
    sfd_dcl->count--;
    sfd_dcl->socketfds[sfd_dcl->count] = 0;
    sfd_dcl->dcls[sfd_dcl->count] = NULL;
    return true;
}

bool sfd_dcl_delete(struct sfd_dcl_storage *sfd_dcl, int sockfd)
{
    if(sfd_dcl == NULL)
	return false;
    size_t pos;
    if(!find_in_sorted_array(sockfd, sfd_dcl->socketfds, sfd_dcl->size, sfd_dcl->count, &pos))
	return false;
    return sfd_dcl_delete_index(sfd_dcl, pos);
}

bool sfd_dcl_empty_dcl(struct sfd_dcl_storage *sfd_dcl, int sockfd)
{
    if(sfd_dcl == NULL)
	return false;
    size_t pos;
    if(!find_in_sorted_array(sockfd, sfd_dcl->socketfds, sfd_dcl->size, sfd_dcl->count, &pos))
	return false;
    dcl_empty(sfd_dcl->dcls[pos]);
    return true;
}

// returns -1 on array size reached, 1 if 'v' is not unique, 0 if everything's OK, -2 on input data errors
int put_to_sorted_array(int v, int *arr, size_t size, size_t *ref_count, size_t *ref_position, bool insert_duplicate)
{
    if((arr == NULL) || (size == 0) || ( ref_position == NULL) || (ref_count == NULL))
	return -2;

    size_t count = *ref_count;
    size_t pos = 0, i;
    bool found = false;

    if(count == 0) {
	arr[0] = v;
	pos = 0;
	count = 1;
    }
    
    else {
	found = find_in_sorted_array(v, arr, size, count, &pos);
	if(found && (!insert_duplicate)) {
	    //do nothing
	}
	else {
	    if(count >= size) {
		msyslog(LOG_ERR, "Failed to add an item into a sorted array: count:%d = size:%d", count, size);
		*ref_position = 0;
		return -1;
	    }
	    if(pos < count) {
		for(i = count; i > pos; i--) 
		    arr[i] = arr[i - 1];
	    }
	    count++;
	    arr[pos] = v;
	}
    }

    *ref_position = pos;
    *ref_count = count;
    return found ? 1 : 0;
}

// ref_position will always contain the index to put 'v'; returns true if 'v' already exists in array, false otherwise 
bool find_in_sorted_array(int v, int *arr, size_t size, size_t count, size_t *ref_position)
{
    if((arr == NULL) || (ref_position == NULL))
	return false;
    bool found = false; // found == true means that value 'v' exists in array in arr[*ref_position]
    bool finished = false; // (finished == true) && (found == false) means that value 'v' doesn't exist in array,
			    // but *ref_position contains the position to put 'v'
    size_t start = 0;
    if(count == 0) {
	*ref_position = 0;
	return false;
    }
    size_t end = count - 1;
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
	    pos = start;
	}
	else if(v == arr[end]) {
	    found = true;
	    pos = end;
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
    *ref_position = pos;
    return found;
}
