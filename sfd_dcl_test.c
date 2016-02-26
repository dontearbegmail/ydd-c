#include "general.h"
#include "test.h"
#include <stdio.h>
#include "datachunks.h"

void do_sfd_dcl_test() 
{
    int SFD_DCL_TEST_SIZE = 14;
    struct dcls_test {
	size_t pos;
	char **chunks;
	size_t length;
    };

    struct sfds_test {
	size_t i;
	int sockfd;
    };

    struct sfd_dcl_test {
	int action;
	int in_add_sockfd;
	char *in_add_chunk;

	size_t out_count;
	struct sfds_test *out_sockfds;
	size_t out_sockfds_count;
	struct dcls_test *out_dcls;
	size_t out_dcls_count;
    };

    const int ACTION_ADD = 1;
    const int ACTION_CREATE = 0;
    struct sfd_dcl_test a1 = {
	ACTION_CREATE,
	0,
	NULL,

	0,
	NULL, 
	0,
	NULL,
	0
    };

    struct sfds_test a2_sfds[] = {{0, 3}};
    char *a2_chunks[] = {"a2"};
    struct dcls_test a2_dcls[] = {{0, a2_chunks, 1}};
    struct sfd_dcl_test a2 = {
	ACTION_ADD,
	3,
	"a2",

	1,
	a2_sfds,
	1,
	a2_dcls,
	1
    };

    struct sfds_test a3_sfds[] = {{0, 3}};
    char *a3_chunks[] = {"a2", "a3"};
    struct dcls_test a3_dcls[] = {{0, a3_chunks, 2}};
    struct sfd_dcl_test a3 = {
	ACTION_ADD,
	3,
	"a3",

	1,
	a3_sfds,
	1,
	a3_dcls,
	1
    };

    struct sfds_test a4_sfds[] = {{0, 3}, {1, 6}};
    char *a4_1_chunks[] = {"a2", "a3"};
    char *a4_2_chunks[] = {"a4"};
    struct dcls_test a4_dcls[] = {
	{0, a4_1_chunks, 2},
	{1, a4_2_chunks, 1}
    };
    struct sfd_dcl_test a4 = {
	ACTION_ADD,
	6,
	"a4",

	2,
	a4_sfds,
	2,
	a4_dcls,
	2
    };


    struct sfds_test a5_sfds[] = {{0, 3}, {1, 5}, {2, 6}};
    char *a5_1_chunks[] = {"a2", "a3"};
    char *a5_2_chunks[] = {"a5"};
    char *a5_3_chunks[] = {"a4"};
    struct dcls_test a5_dcls[] = {
	{0, a5_1_chunks, 2},
	{1, a5_2_chunks, 1},
	{2, a5_3_chunks, 1}
    };
    struct sfd_dcl_test a5 = {
	ACTION_ADD,
	5,
	"a5",

	3,
	a5_sfds,
	3,
	a5_dcls,
	3
    };


    char *a6_3_chunks[] = {"a4", "a6"};
    struct dcls_test a6_dcls[] = {
	{0, a5_1_chunks, 2},
	{1, a5_2_chunks, 1},
	{2, a6_3_chunks, 2}
    };
    struct sfd_dcl_test a6 = {
	ACTION_ADD,
	6,
	"a6",

	3,
	a5_sfds,
	3,
	a6_dcls,
	3
    };

    struct sfds_test a7_sfds[] = {{0, 1}, {1, 3}, {2, 5}, {3, 6}};
    char *a7_0_chunks[] = {"a7"};
    struct dcls_test a7_dcls[] = {
	{0, a7_0_chunks, 1},
	{1, a5_1_chunks, 2},
	{2, a5_2_chunks, 1},
	{3, a6_3_chunks, 2}
    };
    struct sfd_dcl_test a7 = {
	ACTION_ADD,
	1,
	"a7",

	4,
	a7_sfds,
	4,
	a7_dcls,
	4
    };

    void print_test(struct sfd_dcl_test *t)
    {
	assert(t != NULL);
	
	printf("Action: ");
	if(t->action == ACTION_CREATE) 
	    printf("create");
	else if(t->action == ACTION_ADD) 
	    printf("add");
	else
	    printf("(unspecified)");
	printf("\n");

	printf("in_add_sockfd: %d\n", t->in_add_sockfd);
	printf("in_add_chunk: %s\n", t->in_add_chunk);
	printf("out_count: %d\n", t->out_count);
	
	size_t i;
	printf("out_sockfds: ");
	if(t->out_sockfds != NULL) {
	    for(i = 0; i < t->out_sockfds_count; i++) 
		printf("%d => %d, ", t->out_sockfds[i].i, t->out_sockfds[i].sockfd);
	    printf("\n");
	}
	else
	    printf(" (null)\n");
	printf("out_sockfds_count: %d\n", t->out_sockfds_count);

	printf("out_dcls:\n ");
	if(t->out_dcls != NULL) {
	    for(i = 0; i < t->out_dcls_count; i++) {
		printf("\tpos: %d, length: %d, chunks: ", t->out_dcls[i].pos, t->out_dcls[i].length);
		size_t j;
		for(j = 0; j < t->out_dcls[i].length; j++)
			printf("%.*s || ", DATA_CHUNK_SIZE, t->out_dcls[i].chunks[j]);
		printf("\n");
	    }
	}
	else
	    printf(" (null)\n");
	printf("out_dcls_count: %d\n", t->out_dcls_count);
    }

    void print_sfd_dcl(struct sfd_dcl_storage *sds)
    {
	assert(sds != NULL);
	size_t i;
	printf("----------sfd_dcl----------\n");
	printf("size: %d, count %d\n", sds->size, sds->count);
	if(sds->count > 0) {
	    printf("socketfds: ");
	    for(i = 0; i < sds->count; i++) 
		printf("%d => %d, ", i, sds->socketfds[i]);
	    printf("\ndatachunks: ");
	    struct data_chunks_list *dcl;
	    struct data_chunk *dc;
	    for(i = 0; i < sds->count; i++) {
		dcl = sds->dcls[i];
		printf("\n\t%d => {", i);
		dc = dcl->first;
		while(dc != NULL) {
		    printf("%.*s, ", DATA_CHUNK_SIZE, dc->chunk);
		    dc = dc->next;
		}
		printf("}");
	    }
	}
	printf("\n----------------------------\n");
    }

    bool check_result(struct sfd_dcl_test *t, struct sfd_dcl_storage *sds)
    {
	if(sds->count != t->out_count) {
	    printf("Test failed: count doesn't match: test->out_count = %d vs sds->count = %d\n", t->out_count, sds->count);
	    return false;
	}

	size_t i;
	size_t test_i;
	for(i = 0; i < t->out_sockfds_count; i++) {
	    test_i = t->out_sockfds[i].i; 
	    if(test_i < sds->count) {
		if(sds->socketfds[test_i] != t->out_sockfds[i].sockfd) {
		    printf("Test failed: t->out_sockfds[%d] states that sds->socketfds[%d] must "
			    "equal %d, but in fact it equals %d", i, test_i, t->out_sockfds[i].sockfd, sds->socketfds[test_i]);
		    return false;
		}
	    }
	    else {
		printf("Test failed: sockfd test index is out of sockfds storage range: "
			"t->out_sockfds index i = %d. t->out_sockfds[i].i = %d >= sds->count = %d", i, test_i, sds->count);
		return false;
	    }
	}

	struct data_chunks_list *dcl;
	struct data_chunk *dc;
	size_t actual_count;
	bool mismatch;
	for(i = 0; i < t->out_dcls_count; i++) {
	    test_i = t->out_dcls[i].pos;
	    if(test_i < sds->count) {
		dcl = sds->dcls[test_i];
		if(dcl->length != t->out_dcls[i].length) {
		    printf("Test failed: t->out_dcls[%d].length = %d doesn't match"
			    "sds->dcls[%d].length = %d", i, t->out_dcls[i].length, test_i, dcl->length);
		    return false;
		}
		actual_count = 0;
		dc = dcl->first;
		mismatch = false;
		while((dc != NULL) && (!mismatch) && (actual_count < t->out_dcls[i].length)) {
		    char *s = t->out_dcls[i].chunks[actual_count];
		    if(memcmp(dc->chunk, s, strlen(s)) != 0) {
			mismatch = true;
			printf("Test failed: found a mismatch in datachunks test between t->out_dcls[%d]."
				"chunks[%d] = \"%s\" and dcl{%d} = \"%s\"", i, actual_count, s, 
				actual_count, dc->chunk);
		    }
		    else {
			actual_count++;
			dc = dc->next;
		    }
		}
		if(mismatch)
		    return false;
		if(actual_count < t->out_dcls[i].length) {
		    printf("Test failed: t->out_dcls[%d] constains more datachunks than the corresponding dcl", i);
		    return false;
		}
	    }
	    else {
	    	printf("Test failed: dcl test index is out of dcls storage range: "
			"t->out_dcls index i = %d. t->out_dcls[i].pos = %d >= sds->count = %d", i, test_i, sds->count);
		return false;
	    }
	}

	return true;
    }


    struct sfd_dcl_test *tests[] = {&a1, &a2, &a3, &a4, &a5, &a6, &a7};

    size_t i;
    size_t nt = 7;
    struct sfd_dcl_storage *sfd_dcl = NULL;


    for(i = 0; i < nt; i++) {	
	if(tests[i]->action == ACTION_CREATE) {
	    if(sfd_dcl != NULL) {
		printf("Error: trying to create a non-empty sfd_dcl on test %d\n", i);
	    }
	    else {	
		sfd_dcl = sfd_dcl_create(SFD_DCL_TEST_SIZE);
	    }
	}
	else if(tests[i]->action == ACTION_ADD) {
	    if(sfd_dcl == NULL) {
		printf("Error: trying to add an element to a NULL sfd_dcl");
		continue;
	    }
	    sfd_dcl_add(sfd_dcl, tests[i]->in_add_sockfd, tests[i]->in_add_chunk, strlen(tests[i]->in_add_chunk));
	}
	printf("\n\nTest #%d\n", i);
	if(check_result(tests[i], sfd_dcl))
	    printf("\n++ passed ++\n");
	else 
	    printf("\n##### FAILED #####\n");

	    print_test(tests[i]);
	    print_sfd_dcl(sfd_dcl);
    }
}
