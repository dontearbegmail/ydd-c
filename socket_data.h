#ifndef DATACHUNKS_H
#define DATACHUNKS_H

#include "general.h"

#define DATA_CHUNK_SIZE 4096 

struct data_chunk {
    char chunk[DATA_CHUNK_SIZE];
    size_t actual_size;	
    struct data_chunk *next;
};

struct socket_data {
    struct data_chunk *first, *last;
    int length;
};

/* dcls[i] contains data_chunks_list for socketfds[i] */
struct sfd_sd_storage {
    int *sfds;
    struct socket_data **sds;
    size_t size;
    size_t count;
};

struct socket_data * sd_create();
void sd_empty(struct socket_data *sd);
void sd_empty_and_kill(struct socket_data *sd);
int sd_add_chunk(struct socket_data *sd, char *chunk, size_t size);
char * sd_get_string(struct socket_data *sd, size_t *ref_size);

struct sfd_sd_storage *sfd_sd_create(size_t size);
void sfd_sd_empty_and_kill(struct sfd_sd_storage *sfd_sd);
int sfd_sd_add(struct sfd_sd_storage *sfd_sd, int sockfd, char *chunk, size_t size, size_t *ref_index);
bool sfd_sd_delete_index(struct sfd_sd_storage *sfd_sd, size_t index);
bool sfd_sd_delete(struct sfd_sd_storage *sfd_sd, int sockfd);
bool sfd_sd_empty_sd(struct sfd_sd_storage *sfd_sd, int sockfd);

bool find_in_sorted_array(int v, int *arr, size_t size, size_t count, size_t *ref_position);
int put_to_sorted_array(int v, int *arr, size_t size, size_t *ref_count, size_t *ref_position, bool insert_duplicate);

#endif /* DATACHUNKS_H */
