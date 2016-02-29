#include "test.h"
#include "datachunks.h"
#include <stdio.h>

void print_dcl(struct data_chunks_list *dcl) {
    printf("\n--> DCL\n");
    if(dcl == NULL) {
	printf("\t(null)\n");
	return;
    }
    printf("\tlength: %d\ndata chunks: { ", dcl->length);
    struct data_chunk *dc = dcl->first;
    while(dc != NULL) {
	if(dc->chunk != NULL)
	    printf("%s, ", dc->chunk);
	else 
	    printf("(null), ");
	dc = dc->next;
    }
    printf("}\n");
}

void do_dcl_test(bool verbose)
{
    const int ACTION_CREATE = 0;
    const int ACTION_ADD = 1;
    const int ACTION_GET_DATA = 2;

    struct dcl_test {
	int action;
	const char *chunk;
	const char **in_chunks;
	size_t in_length;
	const char **out_chunks;
	size_t out_length;
    };

    void print_chunks(const char **chunks, size_t length)
    {
	if(chunks == NULL) {
	    printf("(null)");
	    return;
	}
	if(length == 0) {
	    printf("{}");
	    return;
	}
	printf("{");
	size_t i;
	for(i = 0; i < length; i++) {
	    printf("%s, ", chunks[i] == NULL ? "(null)" : chunks[i]);
	}
	printf("}");
    }

    void print_test(struct dcl_test * t) 
    {
	printf("\n--> TEST:\n");
	if(t == NULL) {
	    printf("NULL");
	    return;
	}

	printf("\taction: ");
	if(t->action == ACTION_CREATE)
	    printf("create");
	else if(t->action == ACTION_ADD)
	    printf("add");
	else if(t->action == ACTION_GET_DATA)
	    printf("get_data");
	else 
	    printf("(unknown)");
	printf("\n");

	printf("\tchunk: ");
	if(t->chunk == NULL)
	    printf("(null)");
	else
	    printf("%s", t->chunk);
	printf("\n");

	printf("\tin_chunks: ");
	print_chunks(t->in_chunks, t->in_length);
	printf("\n\tin_length: %d\n", t->in_length);

	printf("\tout_chunks: ");
	print_chunks(t->out_chunks, t->out_length);
	printf("\n\tout_length: %d\n", t->out_length);
    }

    bool check_result(struct dcl_test *t, struct data_chunks_list *dcl)
    {
	return true;
    }


    const char *a1_in_chunks[] = {};
    const char *a1_out_chunks[] = {};
    struct dcl_test a1 = {
	ACTION_CREATE,
	NULL,
	a1_in_chunks,
	0,
	a1_out_chunks,
	0
    };

    const char *a2_in_chunks[] = {};
    const char *a2_out_chunks[] = {"a2"};
    struct dcl_test a2 = {
	ACTION_ADD,
	"a2",
	a2_in_chunks,
	0,
	a2_out_chunks,
	1
    };

    struct dcl_test *tests[] = {&a1, &a2};
    size_t nt = 2;
    size_t i;
    bool passed;

    for(i = 0; i < nt; i++) {
	if(verbose);
	    printf("########### Test a%d ", i);
	passed = check_result(tests[i], NULL);
	if(verbose) {
	    printf("\n%s", passed ? " ++ passed ++" : " -- FAILED --");
	    print_test(tests[i]);
	    print_dcl(NULL);
	    printf("\n\n");
	}
    }
}
