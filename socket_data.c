#include <stdlib.h>
#include "socket_data.h"
#include "general.h"

struct socket_data * sd_create()
{
    struct socket_data *new_sd = malloc(sizeof(struct socket_data));
    new_sd->first = NULL;
    new_sd->last = NULL;
    new_sd->length = 0;

    return new_sd;
}

void sd_empty(struct socket_data *sd)
{
    struct data_chunk *dc, *next_dc;
    if(sd == NULL)
	return;

    next_dc = sd->first;
    while(next_dc != NULL) {
	dc = next_dc;
	next_dc = dc->next;
	free(dc);
    }
    sd->first = NULL;
    sd->last = NULL;
    sd->length = 0;
}

void sd_empty_and_kill(struct socket_data *sd)
{
    if(sd == NULL)
	return;
    sd_empty(sd);
    free(sd);
}

int sd_add_chunk(struct socket_data *sd, char *chunk, size_t size)
{
    if(sd == NULL)
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
    if(sd->first == NULL) 
	sd->first = dc;
    else 
	sd->last->next = dc;
    sd->last = dc;
    sd->length++;

    return 0;
}

char * sd_get_string(struct socket_data *sd, size_t *ref_size)
{
    if(ref_size != NULL)
	*ref_size = 0;
    if(sd == NULL)
	return NULL;
    if(sd->length == 0)
	return NULL;

    size_t total = 0;
    struct data_chunk *dc = sd->first;
    while(dc != NULL) {
	total += dc->actual_size;
	dc = dc->next;
    }

    total++;
    char *data = malloc(total);
    int start = 0;
    dc = sd->first;
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

struct sfd_sd_storage *sfd_sd_create(size_t size)
{
    if(size == 0)
	return NULL;
    struct sfd_sd_storage *sfd_sd = malloc(sizeof(struct sfd_sd_storage));
    sfd_sd->size = size;
    sfd_sd->count = 0;
    sfd_sd->sfds = malloc(sizeof(int) * size);
    sfd_sd->sds = malloc(sizeof(struct sfd_sd_storage *) * size);

    size_t i;
    for(i = 0; i < size; i++) {
	sfd_sd->sfds[i] = 0;
	sfd_sd->sds[i] = NULL;
    }

    return sfd_sd;
}

void sfd_sd_empty_and_kill(struct sfd_sd_storage *sfd_sd)
{
    if(sfd_sd == NULL)
	return;

    size_t i;
    for(i = 0; i < sfd_sd->count; i++) 
	sd_empty_and_kill(sfd_sd->sds[i]);
    free(sfd_sd->sds);
    free(sfd_sd->sfds);
}

/* returns -1 on array size reached, 1 if 'v' is not unique, 0 if everything's OK, -2 on input data errors
 * if ref_index is not NULL, it will contain the index of sfd and sd
 */
int sfd_sd_add(struct sfd_sd_storage *sfd_sd, int sockfd, char *chunk, size_t size, size_t *ref_index)
{
    if(sfd_sd == NULL)
	return -2;
    size_t pos, i, count = sfd_sd->count;
    int r = put_to_sorted_array(sockfd, sfd_sd->sfds, sfd_sd->size, &count, &pos, false);
    if(ref_index != NULL)
	*ref_index = pos;
    if(r == -1) {
	msyslog(LOG_ERR, "Can't process the new socket since sfd_sd limit reached: size = %d, count = %d", 
		sfd_sd->size, sfd_sd->count);
    }
    else {
	if(r == 0) { /* it means sockfd is a new, so sfd_sd->sfds were shifted right from 'pos' to 'count'
		      *	and now we have to do the same with sfd_sd->sds
		      */
	    if(pos < sfd_sd->count) {
		for(i = sfd_sd->count; i > pos; i--) 
		    sfd_sd->sds[i] = sfd_sd->sds[i - 1];
	    }
	    sfd_sd->sds[pos] = sd_create();
	    sfd_sd->count = count;
	}
	sd_add_chunk(sfd_sd->sds[pos], chunk, size);
    }

    return r;
}

bool sfd_sd_delete_index(struct sfd_sd_storage *sfd_sd, size_t index) 
{
    if(sfd_sd == NULL)
	return false;
    if(index >= sfd_sd->count)
	return false;
    sd_empty_and_kill(sfd_sd->sds[index]);
    size_t pos = index + 1;
    while(pos < sfd_sd->count) {
	sfd_sd->sfds[pos - 1] = sfd_sd->sfds[pos];
	sfd_sd->sds[pos - 1] = sfd_sd->sds[pos];
	pos++;
    }
    sfd_sd->count--;
    sfd_sd->sfds[sfd_sd->count] = 0;
    sfd_sd->sds[sfd_sd->count] = NULL;
    return true;
}

bool sfd_sd_delete(struct sfd_sd_storage *sfd_sd, int sockfd)
{
    if(sfd_sd == NULL)
	return false;
    size_t pos;
    if(!find_in_sorted_array(sockfd, sfd_sd->sfds, sfd_sd->size, sfd_sd->count, &pos))
	return false;
    return sfd_sd_delete_index(sfd_sd, pos);
}

bool sfd_sd_empty_sd(struct sfd_sd_storage *sfd_sd, int sockfd)
{
    if(sfd_sd == NULL)
	return false;
    size_t pos;
    if(!find_in_sorted_array(sockfd, sfd_sd->sfds, sfd_sd->size, sfd_sd->count, &pos))
	return false;
    sd_empty(sfd_sd->sds[pos]);
    return true;
}

/* returns -1 on array size reached, 1 if 'v' is not unique, 0 if everything's OK, -2 on input data errors */
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
	    /*do nothing*/
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

/* ref_position will always contain the index to put 'v'; returns true if 'v' already exists in array, false otherwise */
bool find_in_sorted_array(int v, int *arr, size_t size, size_t count, size_t *ref_position)
{
    if((arr == NULL) || (ref_position == NULL))
	return false;
    bool found = false; /* found == true means that value 'v' exists in array in arr[*ref_position] */
    bool finished = false; /* (finished == true) && (found == false) means that value 'v' doesn't exist in array,
			    * but *ref_position contains the position to put 'v'
			    */
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
	if(v > arr[end]) { /* most searches will typically end here (when adding new) */
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
	    /* now we have arr[start] < v < arr[end] */
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
