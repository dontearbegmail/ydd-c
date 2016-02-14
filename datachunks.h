#ifndef DATACHUNKS_H
#define DATACHUNKS_H

#define DATA_CHUNK_SIZE 4096

struct data_chunk {
	char chunk[DATA_CHUNK_SIZE];
	size_t actual_size;	// 0 if the chunk is the default HTTP_CHUNK_SIZE,
						// otherwise should contain actual size - it only 
						// must be the last chunk in the list
	struct data_chunk *next;
};

struct data_chunks_list {
	struct data_chunk *first, *last;
	int length;
};

struct data_chunks_list * dcl_create();
void dcl_empty_and_kill(struct data_chunks_list *dcl);
int dcl_add_chunk(struct data_chunks_list *dcl, char *chunk, size_t size_below_default);


#endif // DATACHUNKS_H
