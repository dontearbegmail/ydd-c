#ifndef HTTPCHUNKS_H
#define HTTPCHUNKS_H

#define HTTP_CHUNK_SIZE 4096

struct http_chunk {
	char chunk[HTTP_CHUNK_SIZE];
	size_t actual_size;	// 0 if the chunk is the default HTTP_CHUNK_SIZE,
						// otherwise should contain actual size - it only 
						// must be the last chunk in the list
	struct http_chunk *next;
};

struct http_chunks_list {
	struct http_chunk *first, *last;
	int length;
};

struct http_chunks_list * hcl_create();
void hcl_empty_and_kill(struct http_chunks_list *hcl);
int hcl_add_chunk(struct http_chunks_list *hcl, char *chunk, size_t size_below_default);


#endif
