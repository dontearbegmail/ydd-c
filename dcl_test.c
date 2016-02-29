#include "test.h"

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

}
