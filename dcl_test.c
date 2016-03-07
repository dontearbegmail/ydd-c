#include "test.h"
#include "socket_data.h"
#include <stdio.h>
#include <stdlib.h>

void print_sd(struct socket_data *sd) {
    printf("\n--> DCL\n");
    if(sd == NULL) {
	printf("\t(null)\n");
	return;
    }
    printf("\tlength: %d\n\tdata chunks: { ", sd->length);
    struct data_chunk *dc = sd->first;
    while(dc != NULL) {
	if(dc->chunk != NULL)
	    printf("%.*s, ", dc->actual_size, dc->chunk);
	else 
	    printf("(null), ");
	dc = dc->next;
    }
    printf("}\n");
}

void do_sd_test(bool verbose)
{
    const int ACTION_CREATE = 0;
    const int ACTION_ADD = 1;
    const int ACTION_GET_DATA = 2;
    const int ACTION_EMPTY = 3;

    struct sd_test {
	int action;
	char *data;
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

    void print_test(struct sd_test * t) 
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
	else if(t->action == ACTION_EMPTY)
	    printf("empty");
	else 
	    printf("(unknown)");
	printf("\n");

	printf("\tdata: ");
	if(t->data == NULL)
	    printf("(null)");
	else
	    printf("%s", t->data);
	printf("\n");

	printf("\tout_chunks: ");
	print_chunks(t->out_chunks, t->out_length);
	printf("\n\tout_length: %d\n", t->out_length);
    }

    bool check_result(struct sd_test *t, struct socket_data *sd, bool verbose)
    {
	if(t == NULL) {
	    printf("NULL test provided\n");
	    return false;
	}
	if(sd == NULL) {
	    printf("NULL DCL provided\n");
	    return false;
	}

	if(t->out_length != sd->length) {
	    printf("Lengths don't match: t->out_length = %d vs sd->length = %d", t->out_length, sd->length);
	    return false;
	}
	
	if(t->action == ACTION_GET_DATA) {
	    bool mismatch = false;
	    bool size_mismatch = true;
	    size_t size;
	    size_t tdatalen = 0;
	    char *d = sd_get_string(sd, &size);
	    if((d == NULL) || (t->data == NULL)) {
		if(d != t->data)
		    mismatch = true;
	    }
	    else {
		mismatch = memcmp(d, t->data, strlen(t->data)) != 0;
		tdatalen = strlen(t->data) + 1;
		size_mismatch = size != tdatalen;
	    }

	    if(verbose || mismatch) {
		printf("action_get_data compared: t->data = \"%s\" %s sd_get_data(sd) = \"%s\"\n",
		    t->data == NULL ? "(null)" : t->data,
		    mismatch ? "DOESN'T match" : "matches",
		    d == NULL ? "(null)" : d);
		printf("\t\tsd_get_data returned size = %d %s (strlen(t->data) + 1) = %d\n",
			size,
			size_mismatch ? "DOESN'T EQUAL" : "equals",
			tdatalen);
	    }
	    if(d != NULL)
		free(d);
	    if(mismatch)
		return false;
	    else
		return true;
	}
	else {
	    size_t i = 0;
	    struct data_chunk *dc = sd->first;
	    bool mismatch = false;
	    size_t sl = 0;
	    while((i < t->out_length) && (dc != NULL) && (!mismatch)) {
		if((t->out_chunks[i] == NULL) || (dc->chunk == NULL)) {
		    if(t->out_chunks[i] != dc->chunk) 
			mismatch = true;
		}
		else {
		    sl = strlen(t->out_chunks[i]);
		    if(sl != dc->actual_size) 
			mismatch = true;
		    else 
			mismatch = memcmp(t->out_chunks[i], dc->chunk, dc->actual_size) != 0;
		}
		if(!mismatch) {
		    dc = dc->next;
		    i++;
		}
	    }
	    if(mismatch) {
		printf("Found a mismatch between chunks: ");
		printf("t->out_chunks[%d] = %s vs ", i, t->out_chunks[i] == NULL ? "(null)" : t->out_chunks[i]);
		printf("dc->chunk = %.*s", dc->actual_size, dc->chunk == NULL ? "(null)" : dc->chunk);
		if(sl != dc->actual_size) 
		    printf(", different sizes: t->out_chunks[%d] is %d vs dc->chunk is %d", 
			    i, sl, dc->actual_size);
		return false;
	    }
	    else if(i < t->out_length) {
		printf("Got a NULL datachunk while t->out_chunks[%d] contains data \"%s\"",
			i, t->out_chunks[i]);
		return false;
	    }
	}

	return true;
    }


    const char *a1_out_chunks[] = {};
    struct sd_test a1 = {
	ACTION_CREATE,
	NULL,
	a1_out_chunks,
	0
    };

    const char *a2_out_chunks[] = {"a2"};
    struct sd_test a2 = {
	ACTION_ADD,
	"a2",
	a2_out_chunks,
	1
    };

    const char *a3_out_chunks[] = {"a2", "a3"};
    struct sd_test a3 = {
	ACTION_ADD,
	"a3",
	a3_out_chunks,
	2
    };

    struct sd_test a4 = {
	ACTION_ADD,
	NULL,
	a3_out_chunks,
	2
    };

    const char *a5_out_chunks[] = {"a2", "a3", "a5"};
    struct sd_test a5 = {
	ACTION_ADD,
	"a5",
	a5_out_chunks,
	3
    };

    struct sd_test a6 = {
	ACTION_GET_DATA,
	"a2a3a5",
	a5_out_chunks,
	3
    };

    const char *a7_out_chunks[] = {};
    struct sd_test a7 = {
	ACTION_EMPTY,
	NULL,
	a7_out_chunks,
	0
    };

    struct sd_test a8 = {
	ACTION_ADD,
	NULL,
	a7_out_chunks,
	0
    };

    const char *a9_out_chunks[] = {"a9"};
    struct sd_test a9 = {
	ACTION_ADD,
	"a9",
	a9_out_chunks,
	1
    };

    struct sd_test a10 = {
	ACTION_ADD,
	NULL,
	a9_out_chunks,
	1
    };
    
    const char *a11_out_chunks[] = {"a9", "a11"};
    struct sd_test a11 = {
	ACTION_ADD,
	"a11",
	a11_out_chunks,
	2
    };

    const char *a12_out_chunks[] = {"a9", "a11", "a12"};
    struct sd_test a12 = {
	ACTION_ADD,
	"a12",
	a12_out_chunks,
	3
    };

    struct sd_test a13 = {
	ACTION_GET_DATA,
	"a9a11a12",
	a12_out_chunks,
	3
    };

    const char *a14_out_chunks[] = {"a9", "a11", "a12", "a14"};
    struct sd_test a14 = {
	ACTION_ADD,
	"a14",
	a14_out_chunks,
	4
    };

    struct sd_test a15 = {
	ACTION_ADD,
	NULL,
	a14_out_chunks,
	4
    };

    const char *a16_out_chunks[] = {"a9", "a11", "a12", "a14", "a16"};
    struct sd_test a16 = {
	ACTION_ADD,
	"a16",
	a16_out_chunks,
	5
    };

    struct sd_test a17 = {
	ACTION_GET_DATA,
	"a9a11a12a14a16",
	a16_out_chunks,
	5
    };

    const char *a18_out_chunks[] = {};
    struct sd_test a18 = {
	ACTION_EMPTY,
	NULL,
	a18_out_chunks,
	0
    };


    struct sd_test *tests[] = {&a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9, &a10,
	&a11, &a12, &a13, &a14, &a15, &a16, &a17, &a18};
    size_t nt = 18;
    size_t i;
    bool passed = true;
    bool total_passed = true;
    struct socket_data *sd = NULL;

    for(i = 0; i < nt; i++) {
	if(verbose)
	    printf("########### Test a%d \n", i);

	if(tests[i]->action == ACTION_CREATE) {
	    if(sd != NULL) {
		printf("\nTrying to create a DCL, but it already exists. Have to empty and kill it first\n\n");
		continue;
	    }
	    sd = sd_create();
	}
	else if(tests[i]->action == ACTION_ADD) {
	    if(sd == NULL) {
		printf("\nTrying to ACTION_ADD into a NULL DCL\n\n");
		continue;
	    }
	    sd_add_chunk(sd, tests[i]->data, tests[i]->data == NULL ? 0 : strlen(tests[i]->data));
	}
	else if(tests[i]->action == ACTION_GET_DATA) {
	    if(sd == NULL) {
		printf("\nTrying to get data from a NULL DCL\n\n");
		continue;
	    }
	}
	else if(tests[i]->action == ACTION_EMPTY) {
	    if(sd == NULL) {
		printf("\nTrying to empty a NULL DCL\n\n");
		continue;
	    }
	    sd_empty(sd);
	}

	passed = check_result(tests[i], sd, verbose);
	if(verbose) {
	    printf("\n%s", passed ? "++ passed ++" : "-- FAILED --");
	    print_test(tests[i]);
	    print_sd(sd);
	    printf("\n\n");
	}
	if(!passed)
	    total_passed = false;
    }
    if(!verbose && !total_passed) 
	printf("One of sd_test.c tests failed. Turn on verbose mode for details\n");
    
    if(sd != NULL)
	sd_empty_and_kill(sd);
}
