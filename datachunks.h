#ifndef DATACHUNKS_H
#define DATACHUNKS_H

#include "general.h"

#define DATA_CHUNK_SIZE 4096 

struct data_chunk {
    char chunk[DATA_CHUNK_SIZE];
    size_t actual_size;	
    struct data_chunk *next;
};

struct data_chunks_list {
    struct data_chunk *first, *last;
    int length;
};

/* dcls[i] contains data_chunks_list for socketfds[i] */
struct sfd_dcl_storage {
    int *socketfds;
    struct data_chunks_list **dcls;
    size_t size;
    size_t count;
};

struct data_chunks_list * dcl_create();
void dcl_empty(struct data_chunks_list *dcl);
void dcl_empty_and_kill(struct data_chunks_list *dcl);
int dcl_add_chunk(struct data_chunks_list *dcl, char *chunk, size_t size_below_default);
char * dcl_get_data(struct data_chunks_list *dcl, size_t *ref_size);

struct sfd_dcl_storage *sfd_dcl_create(size_t size);
void sfd_dcl_empty_and_kill(struct sfd_dcl_storage *sfd_dcl);
int sfd_dcl_add(struct sfd_dcl_storage *sfd_dcl, int sockfd, char *chunk, size_t size, size_t *ref_index);
bool sfd_dcl_delete_index(struct sfd_dcl_storage *sfd_dcl, size_t index);
bool sfd_dcl_delete(struct sfd_dcl_storage *sfd_dcl, int sockfd);
bool sfd_dcl_empty_dcl(struct sfd_dcl_storage *sfd_dcl, int sockfd);

bool find_in_sorted_array(int v, int *arr, size_t size, size_t count, size_t *ref_position);
int put_to_sorted_array(int v, int *arr, size_t size, size_t *ref_count, size_t *ref_position, bool insert_duplicate);

#endif /* DATACHUNKS_H */
