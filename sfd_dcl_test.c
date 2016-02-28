#include "general.h"
#include "test.h"
#include <stdio.h>
#include "datachunks.h"

void do_sfd_dcl_test(bool verbose) 
{
    int SFD_DCL_TEST_SIZE = 7;
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
    const int ACTION_DELETE_INDEX = 2;
    const int ACTION_DELETE = 3;
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

    struct sfds_test a2_sfds[] = {{0, 30}};
    char *a2_chunks[] = {"a2"};
    struct dcls_test a2_dcls[] = {{0, a2_chunks, 1}};
    struct sfd_dcl_test a2 = {
	ACTION_ADD,
	30,
	"a2",

	1,
	a2_sfds,
	1,
	a2_dcls,
	1
    };

    struct sfds_test a3_sfds[] = {{0, 30}};
    char *a3_chunks[] = {"a2", "a3"};
    struct dcls_test a3_dcls[] = {{0, a3_chunks, 2}};
    struct sfd_dcl_test a3 = {
	ACTION_ADD,
	30,
	"a3",

	1,
	a3_sfds,
	1,
	a3_dcls,
	1
    };

    struct sfds_test a4_sfds[] = {{0, 30}, {1, 60}};
    char *a4_1_chunks[] = {"a2", "a3"};
    char *a4_2_chunks[] = {"a4"};
    struct dcls_test a4_dcls[] = {
	{0, a4_1_chunks, 2},
	{1, a4_2_chunks, 1}
    };
    struct sfd_dcl_test a4 = {
	ACTION_ADD,
	60,
	"a4",

	2,
	a4_sfds,
	2,
	a4_dcls,
	2
    };


    struct sfds_test a5_sfds[] = {{0, 30}, {1, 50}, {2, 60}};
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
	50,
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
	60,
	"a6",

	3,
	a5_sfds,
	3,
	a6_dcls,
	3
    };

    struct sfds_test a7_sfds[] = {{0, 10}, {1, 30}, {2, 50}, {3, 60}};
    char *a7_0_chunks[] = {"a7"};
    struct dcls_test a7_dcls[] = {
	{0, a7_0_chunks, 1},
	{1, a5_1_chunks, 2},
	{2, a5_2_chunks, 1},
	{3, a6_3_chunks, 2}
    };
    struct sfd_dcl_test a7 = {
	ACTION_ADD,
	10,
	"a7",

	4,
	a7_sfds,
	4,
	a7_dcls,
	4
    };

    char *a8_2_chunks[] = {"a5", "a8"};
    struct dcls_test a8_dcls[] = {
	{0, a7_0_chunks, 1},
	{1, a5_1_chunks, 2},
	{2, a8_2_chunks, 2},
	{3, a6_3_chunks, 2}
    };
    struct sfd_dcl_test a8 = {
	ACTION_ADD,
	50,
	"a8",

	4,
	a7_sfds,
	4,
	a8_dcls,
	4
    };

    struct sfds_test a9_sfds[] = {{0, 10}, {1, 30}, {2, 50}, {3, 60}};
    char *a9_0_chunks[] = {"a7", "a9"};
    char *a9_1_chunks[] = {"a2", "a3"};
    char *a9_2_chunks[] = {"a5", "a8"};
    char *a9_3_chunks[] = {"a4", "a6"};
    struct dcls_test a9_dcls[] = {
	{0, a9_0_chunks, 2},
	{1, a9_1_chunks, 2},
	{2, a9_2_chunks, 2},
	{3, a9_3_chunks, 2}
    };
    struct sfd_dcl_test a9 = {
	ACTION_ADD, 
	10,
	"a9",

	4,
	a9_sfds,
	4,
	a9_dcls,
	4
    };

    struct sfds_test a10_sfds[] = {{0, 10}, {1, 30}, {2, 40}, {3, 50}, {4, 60}};
    char *a10_0_chunks[] = {"a7", "a9"};
    char *a10_1_chunks[] = {"a2", "a3"};
    char *a10_2_chunks[] = {"a10"};
    char *a10_3_chunks[] = {"a5", "a8"};
    char *a10_4_chunks[] = {"a4", "a6"};
    struct dcls_test a10_dcls[] = {
	{0, a10_0_chunks, 2},
	{1, a10_1_chunks, 2},
	{2, a10_2_chunks, 1},
	{3, a10_3_chunks, 2},
	{4, a10_4_chunks, 2}
    };
    struct sfd_dcl_test a10 = {
	ACTION_ADD, 
	40,
	"a10",

	5,
	a10_sfds,
	5,
	a10_dcls,
	5
    };

    struct sfds_test a11_sfds[] = {{0, 10}, {1, 30}, {2, 40}, {3, 50}, {4, 60}, {5, 100}};
    char *a11_0_chunks[] = {"a7", "a9"};
    char *a11_1_chunks[] = {"a2", "a3"};
    char *a11_2_chunks[] = {"a10"};
    char *a11_3_chunks[] = {"a5", "a8"};
    char *a11_4_chunks[] = {"a4", "a6"};
    char *a11_5_chunks[] = {"a11"};
    struct dcls_test a11_dcls[] = {
	{0, a11_0_chunks, 2},
	{1, a11_1_chunks, 2},
	{2, a11_2_chunks, 1},
	{3, a11_3_chunks, 2},
	{4, a11_4_chunks, 2},
	{5, a11_5_chunks, 1}
    };
    struct sfd_dcl_test a11 = {
	ACTION_ADD, 
	100,
	"a11",

	6,
	a11_sfds,
	6,
	a11_dcls,
	6
    };

    struct sfds_test a12_sfds[] = {{0, 10}, {1, 30}, {2, 40}, {3, 50}, {4, 60}, {5, 100}};
    char *a12_0_chunks[] = {"a7", "a9"};
    char *a12_1_chunks[] = {"a2", "a3"};
    char *a12_2_chunks[] = {"a10", "a12"};
    char *a12_3_chunks[] = {"a5", "a8"};
    char *a12_4_chunks[] = {"a4", "a6"};
    char *a12_5_chunks[] = {"a11"};
    struct dcls_test a12_dcls[] = {
	{0, a12_0_chunks, 2},
	{1, a12_1_chunks, 2},
	{2, a12_2_chunks, 2},
	{3, a12_3_chunks, 2},
	{4, a12_4_chunks, 2},
	{5, a12_5_chunks, 1}
    };
    struct sfd_dcl_test a12 = {
	ACTION_ADD, 
	40,
	"a12",

	6,
	a12_sfds,
	6,
	a12_dcls,
	6
    };

    struct sfds_test a13_sfds[] = {{0, 10}, {1, 30}, {2, 40}, {3, 50}, {4, 60}, {5, 100}};
    char *a13_0_chunks[] = {"a7", "a9"};
    char *a13_1_chunks[] = {"a2", "a3"};
    char *a13_2_chunks[] = {"a10", "a12"};
    char *a13_3_chunks[] = {"a5", "a8", "a13"};
    char *a13_4_chunks[] = {"a4", "a6"};
    char *a13_5_chunks[] = {"a11"};
    struct dcls_test a13_dcls[] = {
	{0, a13_0_chunks, 2},
	{1, a13_1_chunks, 2},
	{2, a13_2_chunks, 2},
	{3, a13_3_chunks, 3},
	{4, a13_4_chunks, 2},
	{5, a13_5_chunks, 1}
    };
    struct sfd_dcl_test a13 = {
	ACTION_ADD, 
	50,
	"a13",

	6,
	a13_sfds,
	6,
	a13_dcls,
	6
    };
    
    struct sfds_test a14_sfds[] = {{0, 10}, {1, 30}, {2, 40}, {3, 50}, {4, 60}, {5, 100}};
    char *a14_0_chunks[] = {"a7", "a9"};
    char *a14_1_chunks[] = {"a2", "a3", "a14"};
    char *a14_2_chunks[] = {"a10", "a12"};
    char *a14_3_chunks[] = {"a5", "a8", "a13"};
    char *a14_4_chunks[] = {"a4", "a6"};
    char *a14_5_chunks[] = {"a11"};
    struct dcls_test a14_dcls[] = {
	{0, a14_0_chunks, 2},
	{1, a14_1_chunks, 3},
	{2, a14_2_chunks, 2},
	{3, a14_3_chunks, 3},
	{4, a14_4_chunks, 2},
	{5, a14_5_chunks, 1}
    };
    struct sfd_dcl_test a14 = {
	ACTION_ADD, 
	30,
	"a14",

	6,
	a14_sfds,
	6,
	a14_dcls,
	6
    };

    struct sfds_test a15_sfds[] = {{0, 10}, {1, 30}, {2, 40}, {3, 50}, {4, 60}, {5, 100}};
    char *a15_0_chunks[] = {"a7", "a9"};
    char *a15_1_chunks[] = {"a2", "a3", "a14"};
    char *a15_2_chunks[] = {"a10", "a12"};
    char *a15_3_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a15_4_chunks[] = {"a4", "a6"};
    char *a15_5_chunks[] = {"a11"};
    struct dcls_test a15_dcls[] = {
	{0, a15_0_chunks, 2},
	{1, a15_1_chunks, 3},
	{2, a15_2_chunks, 2},
	{3, a15_3_chunks, 4},
	{4, a15_4_chunks, 2},
	{5, a15_5_chunks, 1}
    };
    struct sfd_dcl_test a15 = {
	ACTION_ADD, 
	50,
	"a15",

	6,
	a15_sfds,
	6,
	a15_dcls,
	6
    };

    struct sfds_test a16_sfds[] = {{0, 10}, {1, 20}, {2, 30}, {3, 40}, {4, 50}, {5, 60}, {6, 100}};
    char *a16_0_chunks[] = {"a7", "a9"};
    char *a16_1_chunks[] = {"a16"};
    char *a16_2_chunks[] = {"a2", "a3", "a14"};
    char *a16_3_chunks[] = {"a10", "a12"};
    char *a16_4_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a16_5_chunks[] = {"a4", "a6"};
    char *a16_6_chunks[] = {"a11"};
    struct dcls_test a16_dcls[] = {
	{0, a16_0_chunks, 2},
	{1, a16_1_chunks, 1},
	{2, a16_2_chunks, 3},
	{3, a16_3_chunks, 2},
	{4, a16_4_chunks, 4},
	{5, a16_5_chunks, 2},
	{6, a16_6_chunks, 1}
    };
    struct sfd_dcl_test a16 = {
	ACTION_ADD, 
	20,
	"a16",

	7,
	a16_sfds,
	7,
	a16_dcls,
	7
    };

    struct sfds_test a17_sfds[] = {{0, 10}, {1, 20}, {2, 30}, {3, 40}, {4, 50}, {5, 60}, {6, 100}};
    char *a17_0_chunks[] = {"a7", "a9"};
    char *a17_1_chunks[] = {"a16", "a17"};
    char *a17_2_chunks[] = {"a2", "a3", "a14"};
    char *a17_3_chunks[] = {"a10", "a12"};
    char *a17_4_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a17_5_chunks[] = {"a4", "a6"};
    char *a17_6_chunks[] = {"a11"};
    struct dcls_test a17_dcls[] = {
	{0, a17_0_chunks, 2},
	{1, a17_1_chunks, 2},
	{2, a17_2_chunks, 3},
	{3, a17_3_chunks, 2},
	{4, a17_4_chunks, 4},
	{5, a17_5_chunks, 2},
	{6, a17_6_chunks, 1}
    };
    struct sfd_dcl_test a17 = {
	ACTION_ADD, 
	20,
	"a17",

	7,
	a17_sfds,
	7,
	a17_dcls,
	7
    };

    struct sfds_test a18_sfds[] = {{0, 10}, {1, 20}, {2, 30}, {3, 40}, {4, 50}, {5, 60}, {6, 100}};
    char *a18_0_chunks[] = {"a7", "a9"};
    char *a18_1_chunks[] = {"a16", "a17"};
    char *a18_2_chunks[] = {"a2", "a3", "a14"};
    char *a18_3_chunks[] = {"a10", "a12"};
    char *a18_4_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a18_5_chunks[] = {"a4", "a6"};
    char *a18_6_chunks[] = {"a11", "a18"};
    struct dcls_test a18_dcls[] = {
	{0, a18_0_chunks, 2},
	{1, a18_1_chunks, 2},
	{2, a18_2_chunks, 3},
	{3, a18_3_chunks, 2},
	{4, a18_4_chunks, 4},
	{5, a18_5_chunks, 2},
	{6, a18_6_chunks, 2}
    };
    struct sfd_dcl_test a18 = {
	ACTION_ADD, 
	100,
	"a18",

	7,
	a18_sfds,
	7,
	a18_dcls,
	7
    };

    struct sfd_dcl_test a19 = {
	ACTION_ADD,
	19,
	"a19",

	7,
	a18_sfds,
	7,
	a18_dcls,
	7
    };

    struct sfd_dcl_test a20 = {
	ACTION_ADD,
	0,
	"a20",

	7,
	a18_sfds,
	7,
	a18_dcls,
	7
    };

    struct sfd_dcl_test a21 = {
	ACTION_ADD,
	8,
	"a21",

	7,
	a18_sfds,
	7,
	a18_dcls,
	7
    };
    
    struct sfds_test a22_sfds[] = {{0, 10}, {1, 20}, {2, 30}, {3, 40}, {4, 50}, {5, 60}};
    char *a22_0_chunks[] = {"a7", "a9"};
    char *a22_1_chunks[] = {"a16", "a17"};
    char *a22_2_chunks[] = {"a2", "a3", "a14"};
    char *a22_3_chunks[] = {"a10", "a12"};
    char *a22_4_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a22_5_chunks[] = {"a4", "a6"};
    struct dcls_test a22_dcls[] = {
	{0, a22_0_chunks, 2},
	{1, a22_1_chunks, 2},
	{2, a22_2_chunks, 3},
	{3, a22_3_chunks, 2},
	{4, a22_4_chunks, 4},
	{5, a22_5_chunks, 2},
    };
    struct sfd_dcl_test a22 = {
	ACTION_DELETE_INDEX,
	6,
	NULL,

	6,
	a22_sfds,
	6,
	a22_dcls,
	6
    };

    struct sfd_dcl_test a23 = {
	ACTION_DELETE_INDEX,
	10,
	NULL,

	6,
	a22_sfds,
	6,
	a22_dcls,
	6
    };

    struct sfds_test a24_sfds[] = {{0, 10}, {1, 20}, {2, 30}, {3, 35}, {4, 40}, {5, 50}, {6, 60}};
    char *a24_0_chunks[] = {"a7", "a9"};
    char *a24_1_chunks[] = {"a16", "a17"};
    char *a24_2_chunks[] = {"a2", "a3", "a14"};
    char *a24_3_chunks[] = {"a24"};
    char *a24_4_chunks[] = {"a10", "a12"};
    char *a24_5_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a24_6_chunks[] = {"a4", "a6"};
    struct dcls_test a24_dcls[] = {
	{0, a24_0_chunks, 2},
	{1, a24_1_chunks, 2},
	{2, a24_2_chunks, 3},
	{3, a24_3_chunks, 1},
	{4, a24_4_chunks, 2},
	{5, a24_5_chunks, 4},
	{6, a24_6_chunks, 2},
    };
    struct sfd_dcl_test a24 = {
	ACTION_ADD,
	35,
	"a24",

	7,
	a24_sfds,
	7,
	a24_dcls,
	7
    };

    struct sfds_test a25_sfds[] = {{0, 20}, {1, 30}, {2, 35}, {3, 40}, {4, 50}, {5, 60}};
    char *a25_0_chunks[] = {"a16", "a17"};
    char *a25_1_chunks[] = {"a2", "a3", "a14"};
    char *a25_2_chunks[] = {"a24"};
    char *a25_3_chunks[] = {"a10", "a12"};
    char *a25_4_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a25_5_chunks[] = {"a4", "a6"};
    struct dcls_test a25_dcls[] = {
	{0, a25_0_chunks, 2},
	{1, a25_1_chunks, 3},
	{2, a25_2_chunks, 1},
	{3, a25_3_chunks, 2},
	{4, a25_4_chunks, 4},
	{5, a25_5_chunks, 2},
    };
    struct sfd_dcl_test a25 = {
	ACTION_DELETE_INDEX,
	0,
	NULL,

	6,
	a25_sfds,
	6,
	a25_dcls,
	6
    };

    struct sfds_test a26_sfds[] = {{0, 30}, {1, 35}, {2, 40}, {3, 50}, {4, 60}};
    char *a26_1_chunks[] = {"a2", "a3", "a14"};
    char *a26_2_chunks[] = {"a24"};
    char *a26_3_chunks[] = {"a10", "a12"};
    char *a26_4_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a26_5_chunks[] = {"a4", "a6"};
    struct dcls_test a26_dcls[] = {
	{0, a26_1_chunks, 3},
	{1, a26_2_chunks, 1},
	{2, a26_3_chunks, 2},
	{3, a26_4_chunks, 4},
	{4, a26_5_chunks, 2},
    };
    struct sfd_dcl_test a26 = {
	ACTION_DELETE,
	20,
	NULL,

	5,
	a26_sfds,
	5,
        a26_dcls,
	5	
    };

    struct sfds_test a27_sfds[] = {{0, 30}, {1, 35}, {2, 40}, {3, 45}, {4, 50}, {5, 60}};
    char *a27_1_chunks[] = {"a2", "a3", "a14"};
    char *a27_2_chunks[] = {"a24"};
    char *a27_3_chunks[] = {"a10", "a12"};
    char *a27_4_chunks[] = {"a27"};
    char *a27_5_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a27_6_chunks[] = {"a4", "a6"};
    struct dcls_test a27_dcls[] = {
	{0, a27_1_chunks, 3},
	{1, a27_2_chunks, 1},
	{2, a27_3_chunks, 2},
	{3, a27_4_chunks, 1},
	{4, a27_5_chunks, 4},
	{5, a27_6_chunks, 2},
    };
    struct sfd_dcl_test a27 = {
	ACTION_ADD,
	45,
	"a27",

	6,
	a27_sfds,
	6,
        a27_dcls,
	6	
    };

    struct sfds_test a28_sfds[] = {{0, 30}, {1, 35}, {2, 40}, {3, 45}, {4, 50}, {5, 60}};
    char *a28_1_chunks[] = {"a2", "a3", "a14"};
    char *a28_2_chunks[] = {"a24"};
    char *a28_3_chunks[] = {"a10", "a12"};
    char *a28_4_chunks[] = {"a27", "a28"};
    char *a28_5_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a28_6_chunks[] = {"a4", "a6"};
    struct dcls_test a28_dcls[] = {
	{0, a28_1_chunks, 3},
	{1, a28_2_chunks, 1},
	{2, a28_3_chunks, 2},
	{3, a28_4_chunks, 2},
	{4, a28_5_chunks, 4},
	{5, a28_6_chunks, 2},
    };
    struct sfd_dcl_test a28 = {
	ACTION_ADD,
	45,
	"a28",

	6,
	a28_sfds,
	6,
        a28_dcls,
	6	
    };

    struct sfds_test a29_sfds[] = {{0, 15}, {1, 30}, {2, 35}, {3, 40}, {4, 45}, {5, 50}, {6, 60}};
    char *a29_0_chunks[] = {"a29"};
    char *a29_1_chunks[] = {"a2", "a3", "a14"};
    char *a29_2_chunks[] = {"a24"};
    char *a29_3_chunks[] = {"a10", "a12"};
    char *a29_4_chunks[] = {"a27", "a28"};
    char *a29_5_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a29_6_chunks[] = {"a4", "a6"};
    struct dcls_test a29_dcls[] = {
	{0, a29_0_chunks, 1},
	{1, a29_1_chunks, 3},
	{2, a29_2_chunks, 1},
	{3, a29_3_chunks, 2},
	{4, a29_4_chunks, 2},
	{5, a29_5_chunks, 4},
	{6, a29_6_chunks, 2},
    };
    struct sfd_dcl_test a29 = {
	ACTION_ADD,
	15,
	"a29",

	7,
	a29_sfds,
	7,
        a29_dcls,
	7	
    };

    struct sfd_dcl_test a30 = {
	ACTION_ADD,
	16,
	"a30",

	7,
	a29_sfds,
	7,
	a29_dcls,
	7
    };

    struct sfds_test a31_sfds[] = {{0, 15}, {1, 30}, {2, 35}, {3, 40}, {4, 45}, {5, 50}, {6, 60}};
    char *a31_0_chunks[] = {"a29", "a31"};
    char *a31_1_chunks[] = {"a2", "a3", "a14"};
    char *a31_2_chunks[] = {"a24"};
    char *a31_3_chunks[] = {"a10", "a12"};
    char *a31_4_chunks[] = {"a27", "a28"};
    char *a31_5_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a31_6_chunks[] = {"a4", "a6"};
    struct dcls_test a31_dcls[] = {
	{0, a31_0_chunks, 2},
	{1, a31_1_chunks, 3},
	{2, a31_2_chunks, 1},
	{3, a31_3_chunks, 2},
	{4, a31_4_chunks, 2},
	{5, a31_5_chunks, 4},
	{6, a31_6_chunks, 2},
    };
    struct sfd_dcl_test a31 = {
	ACTION_ADD,
	15,
	"a31",

	7,
	a31_sfds,
	7,
        a31_dcls,
	7	
    };

    struct sfds_test a32_sfds[] = {{0, 15}, {1, 30}, {2, 35}, {3, 40}, {4, 45}, {5, 50}, {6, 60}};
    char *a32_0_chunks[] = {"a29", "a31"};
    char *a32_1_chunks[] = {"a2", "a3", "a14"};
    char *a32_2_chunks[] = {"a24", "a32"};
    char *a32_3_chunks[] = {"a10", "a12"};
    char *a32_4_chunks[] = {"a27", "a28"};
    char *a32_5_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a32_6_chunks[] = {"a4", "a6"};
    struct dcls_test a32_dcls[] = {
	{0, a32_0_chunks, 2},
	{1, a32_1_chunks, 3},
	{2, a32_2_chunks, 2},
	{3, a32_3_chunks, 2},
	{4, a32_4_chunks, 2},
	{5, a32_5_chunks, 4},
	{6, a32_6_chunks, 2},
    };
    struct sfd_dcl_test a32 = {
	ACTION_ADD,
	35,
	"a32",

	7,
	a32_sfds,
	7,
        a32_dcls,
	7	
    };

    struct sfds_test a33_sfds[] = {{0, 15}, {1, 30}, {2, 35}, {3, 45}, {4, 50}, {5, 60}};
    char *a33_0_chunks[] = {"a29", "a31"};
    char *a33_1_chunks[] = {"a2", "a3", "a14"};
    char *a33_2_chunks[] = {"a24", "a32"};
    char *a33_4_chunks[] = {"a27", "a28"};
    char *a33_5_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a33_6_chunks[] = {"a4", "a6"};
    struct dcls_test a33_dcls[] = {
	{0, a33_0_chunks, 2},
	{1, a33_1_chunks, 3},
	{2, a33_2_chunks, 2},
	{3, a33_4_chunks, 2},
	{4, a33_5_chunks, 4},
	{5, a33_6_chunks, 2},
    };
    struct sfd_dcl_test a33 = {
	ACTION_DELETE,
	40,
	NULL,

	6,
	a33_sfds,
	6,
        a33_dcls,
	6	
    };

    struct sfds_test a34_sfds[] = {{0, 15}, {1, 30}, {2, 35}, {3, 45}, {4, 47}, {5, 50}, {6, 60}};
    char *a34_0_chunks[] = {"a29", "a31"};
    char *a34_1_chunks[] = {"a2", "a3", "a14"};
    char *a34_2_chunks[] = {"a24", "a32"};
    char *a34_3_chunks[] = {"a27", "a28"};
    char *a34_4_chunks[] = {};
    char *a34_5_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a34_6_chunks[] = {"a4", "a6"};
    struct dcls_test a34_dcls[] = {
	{0, a34_0_chunks, 2},
	{1, a34_1_chunks, 3},
	{2, a34_2_chunks, 2},
	{3, a34_3_chunks, 2},
	{4, a34_4_chunks, 0},
	{5, a34_5_chunks, 4},
	{6, a34_6_chunks, 2},
    };
    struct sfd_dcl_test a34 = {
	ACTION_ADD,
	47,
	NULL,

	7,
	a34_sfds,
	7,
        a34_dcls,
	7	
    };

    struct sfd_dcl_test a35 = {
	ACTION_ADD,
	33,
	NULL,

	7,
	a34_sfds,
	7,
	a34_dcls,
	7
    };

    struct sfd_dcl_test a36 = {
	ACTION_ADD,
	47,
	NULL,

	7,
	a34_sfds,
	7,
	a34_dcls,
	7
    };

    struct sfds_test a37_sfds[] = {{0, 15}, {1, 30}, {2, 35}, {3, 45}, {4, 47}, {5, 50}, {6, 60}};
    char *a37_0_chunks[] = {"a29", "a31"};
    char *a37_1_chunks[] = {"a2", "a3", "a14"};
    char *a37_2_chunks[] = {"a24", "a32"};
    char *a37_3_chunks[] = {"a27", "a28"};
    char *a37_4_chunks[] = {"a37"};
    char *a37_5_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a37_6_chunks[] = {"a4", "a6"};
    struct dcls_test a37_dcls[] = {
	{0, a37_0_chunks, 2},
	{1, a37_1_chunks, 3},
	{2, a37_2_chunks, 2},
	{3, a37_3_chunks, 2},
	{4, a37_4_chunks, 1},
	{5, a37_5_chunks, 4},
	{6, a37_6_chunks, 2},
    };
    struct sfd_dcl_test a37 = {
	ACTION_ADD,
	47,
	"a37",

	7,
	a37_sfds,
	7,
        a37_dcls,
	7	
    };

    void print_test(struct sfd_dcl_test *t)
    {
	assert(t != NULL);
	
	printf("Action: ");
	if(t->action == ACTION_CREATE) 
	    printf("create");
	else if(t->action == ACTION_ADD) 
	    printf("add");
	else if(t->action == ACTION_DELETE_INDEX)
	    printf("delete_index");
	else if(t->action == ACTION_DELETE) 
	    printf("delete");
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
	    for(i = 0; i < sds->size; i++) 
		printf("%d => %d, ", i, sds->socketfds[i]);
	    printf("\ndatachunks: ");
	    struct data_chunks_list *dcl;
	    struct data_chunk *dc;
	    for(i = 0; i < sds->size; i++) {
		dcl = sds->dcls[i];
		printf("\n\t%d => {", i);
		if(dcl != NULL) {
		    dc = dcl->first;
		    while(dc != NULL) {
			printf("%.*s, ", DATA_CHUNK_SIZE, dc->chunk);
			dc = dc->next;
		    }
		}
		else
		    printf(" (null) ");
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


    struct sfd_dcl_test *tests[] = {&a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9, 
	&a10, &a11, &a12, &a13, &a14, &a15, &a16, &a17, &a18, &a19, &a20, &a21, 
	&a22, &a23, &a24, &a25, &a26, &a27, &a28, &a29, &a30, &a31, &a32, &a33, 
	&a34, &a35, &a36, &a37};

    size_t i;
    size_t nt = 37;
    struct sfd_dcl_storage *sfd_dcl = NULL;
    bool failure = false;

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
	    size_t sl = 0;
	    if(tests[i]->in_add_chunk != NULL)
		sl = strlen(tests[i]->in_add_chunk) + 1;
	    sfd_dcl_add(sfd_dcl, tests[i]->in_add_sockfd, tests[i]->in_add_chunk, sl);
	}
	else if(tests[i]->action == ACTION_DELETE_INDEX) {
	    if(sfd_dcl == NULL) {
		printf("Error: trying to remove an element from a NULL sfd_dcl");
		continue;
	    }
	    sfd_dcl_delete_index(sfd_dcl, tests[i]->in_add_sockfd);
	}
	else if(tests[i]->action == ACTION_DELETE) {
	    if(sfd_dcl == NULL) {
		printf("Error: trying to remove an element from a NULL sfd_dcl");
		continue;
	    }
	    sfd_dcl_delete(sfd_dcl, tests[i]->in_add_sockfd);
	}
	failure = !check_result(tests[i], sfd_dcl);
	if(verbose) {
	    printf("\n\nTest #%d\n", i);
	    if(!failure) 
		printf("\n++ passed ++\n");
	    else  
		printf("\n##### FAILED #####\n");

	    print_test(tests[i]);
	    print_sfd_dcl(sfd_dcl);
	}
	if(failure && (!verbose)) {
	    printf("\n!!!!!!!!! One or more tests in do_sfd_dcl_test failed. Call it with verbose = true for details\n");
	}
    }

    if(sfd_dcl != NULL)
	sfd_dcl_empty_and_kill(sfd_dcl);
}
