#include "general.h"
#include "test.h"
#include <stdio.h>
#include "socket_data.h"

void do_sfd_sd_test(bool verbose) 
{
    int SFD_DCL_TEST_SIZE = 7;
    struct sds_test {
	size_t pos;
	char **chunks;
	size_t length;
    };

    struct sfds_test {
	size_t i;
	int sockfd;
    };

    struct sfd_sd_test {
	int action;
	int in_add_sockfd;
	char *in_add_chunk;

	size_t out_count;
	struct sfds_test *out_sockfds;
	size_t out_sockfds_count;
	struct sds_test *out_sds;
	size_t out_sds_count;
    };

    const int ACTION_ADD = 1;
    const int ACTION_CREATE = 0;
    const int ACTION_DELETE_INDEX = 2;
    const int ACTION_DELETE = 3;
    const int ACTION_EMPTY_DCL = 4;
    struct sfd_sd_test a1 = {
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
    struct sds_test a2_sds[] = {{0, a2_chunks, 1}};
    struct sfd_sd_test a2 = {
	ACTION_ADD,
	30,
	"a2",

	1,
	a2_sfds,
	1,
	a2_sds,
	1
    };

    struct sfds_test a3_sfds[] = {{0, 30}};
    char *a3_chunks[] = {"a2", "a3"};
    struct sds_test a3_sds[] = {{0, a3_chunks, 2}};
    struct sfd_sd_test a3 = {
	ACTION_ADD,
	30,
	"a3",

	1,
	a3_sfds,
	1,
	a3_sds,
	1
    };

    struct sfds_test a4_sfds[] = {{0, 30}, {1, 60}};
    char *a4_1_chunks[] = {"a2", "a3"};
    char *a4_2_chunks[] = {"a4"};
    struct sds_test a4_sds[] = {
	{0, a4_1_chunks, 2},
	{1, a4_2_chunks, 1}
    };
    struct sfd_sd_test a4 = {
	ACTION_ADD,
	60,
	"a4",

	2,
	a4_sfds,
	2,
	a4_sds,
	2
    };


    struct sfds_test a5_sfds[] = {{0, 30}, {1, 50}, {2, 60}};
    char *a5_1_chunks[] = {"a2", "a3"};
    char *a5_2_chunks[] = {"a5"};
    char *a5_3_chunks[] = {"a4"};
    struct sds_test a5_sds[] = {
	{0, a5_1_chunks, 2},
	{1, a5_2_chunks, 1},
	{2, a5_3_chunks, 1}
    };
    struct sfd_sd_test a5 = {
	ACTION_ADD,
	50,
	"a5",

	3,
	a5_sfds,
	3,
	a5_sds,
	3
    };


    char *a6_3_chunks[] = {"a4", "a6"};
    struct sds_test a6_sds[] = {
	{0, a5_1_chunks, 2},
	{1, a5_2_chunks, 1},
	{2, a6_3_chunks, 2}
    };
    struct sfd_sd_test a6 = {
	ACTION_ADD,
	60,
	"a6",

	3,
	a5_sfds,
	3,
	a6_sds,
	3
    };

    struct sfds_test a7_sfds[] = {{0, 10}, {1, 30}, {2, 50}, {3, 60}};
    char *a7_0_chunks[] = {"a7"};
    struct sds_test a7_sds[] = {
	{0, a7_0_chunks, 1},
	{1, a5_1_chunks, 2},
	{2, a5_2_chunks, 1},
	{3, a6_3_chunks, 2}
    };
    struct sfd_sd_test a7 = {
	ACTION_ADD,
	10,
	"a7",

	4,
	a7_sfds,
	4,
	a7_sds,
	4
    };

    char *a8_2_chunks[] = {"a5", "a8"};
    struct sds_test a8_sds[] = {
	{0, a7_0_chunks, 1},
	{1, a5_1_chunks, 2},
	{2, a8_2_chunks, 2},
	{3, a6_3_chunks, 2}
    };
    struct sfd_sd_test a8 = {
	ACTION_ADD,
	50,
	"a8",

	4,
	a7_sfds,
	4,
	a8_sds,
	4
    };

    struct sfds_test a9_sfds[] = {{0, 10}, {1, 30}, {2, 50}, {3, 60}};
    char *a9_0_chunks[] = {"a7", "a9"};
    char *a9_1_chunks[] = {"a2", "a3"};
    char *a9_2_chunks[] = {"a5", "a8"};
    char *a9_3_chunks[] = {"a4", "a6"};
    struct sds_test a9_sds[] = {
	{0, a9_0_chunks, 2},
	{1, a9_1_chunks, 2},
	{2, a9_2_chunks, 2},
	{3, a9_3_chunks, 2}
    };
    struct sfd_sd_test a9 = {
	ACTION_ADD, 
	10,
	"a9",

	4,
	a9_sfds,
	4,
	a9_sds,
	4
    };

    struct sfds_test a10_sfds[] = {{0, 10}, {1, 30}, {2, 40}, {3, 50}, {4, 60}};
    char *a10_0_chunks[] = {"a7", "a9"};
    char *a10_1_chunks[] = {"a2", "a3"};
    char *a10_2_chunks[] = {"a10"};
    char *a10_3_chunks[] = {"a5", "a8"};
    char *a10_4_chunks[] = {"a4", "a6"};
    struct sds_test a10_sds[] = {
	{0, a10_0_chunks, 2},
	{1, a10_1_chunks, 2},
	{2, a10_2_chunks, 1},
	{3, a10_3_chunks, 2},
	{4, a10_4_chunks, 2}
    };
    struct sfd_sd_test a10 = {
	ACTION_ADD, 
	40,
	"a10",

	5,
	a10_sfds,
	5,
	a10_sds,
	5
    };

    struct sfds_test a11_sfds[] = {{0, 10}, {1, 30}, {2, 40}, {3, 50}, {4, 60}, {5, 100}};
    char *a11_0_chunks[] = {"a7", "a9"};
    char *a11_1_chunks[] = {"a2", "a3"};
    char *a11_2_chunks[] = {"a10"};
    char *a11_3_chunks[] = {"a5", "a8"};
    char *a11_4_chunks[] = {"a4", "a6"};
    char *a11_5_chunks[] = {"a11"};
    struct sds_test a11_sds[] = {
	{0, a11_0_chunks, 2},
	{1, a11_1_chunks, 2},
	{2, a11_2_chunks, 1},
	{3, a11_3_chunks, 2},
	{4, a11_4_chunks, 2},
	{5, a11_5_chunks, 1}
    };
    struct sfd_sd_test a11 = {
	ACTION_ADD, 
	100,
	"a11",

	6,
	a11_sfds,
	6,
	a11_sds,
	6
    };

    struct sfds_test a12_sfds[] = {{0, 10}, {1, 30}, {2, 40}, {3, 50}, {4, 60}, {5, 100}};
    char *a12_0_chunks[] = {"a7", "a9"};
    char *a12_1_chunks[] = {"a2", "a3"};
    char *a12_2_chunks[] = {"a10", "a12"};
    char *a12_3_chunks[] = {"a5", "a8"};
    char *a12_4_chunks[] = {"a4", "a6"};
    char *a12_5_chunks[] = {"a11"};
    struct sds_test a12_sds[] = {
	{0, a12_0_chunks, 2},
	{1, a12_1_chunks, 2},
	{2, a12_2_chunks, 2},
	{3, a12_3_chunks, 2},
	{4, a12_4_chunks, 2},
	{5, a12_5_chunks, 1}
    };
    struct sfd_sd_test a12 = {
	ACTION_ADD, 
	40,
	"a12",

	6,
	a12_sfds,
	6,
	a12_sds,
	6
    };

    struct sfds_test a13_sfds[] = {{0, 10}, {1, 30}, {2, 40}, {3, 50}, {4, 60}, {5, 100}};
    char *a13_0_chunks[] = {"a7", "a9"};
    char *a13_1_chunks[] = {"a2", "a3"};
    char *a13_2_chunks[] = {"a10", "a12"};
    char *a13_3_chunks[] = {"a5", "a8", "a13"};
    char *a13_4_chunks[] = {"a4", "a6"};
    char *a13_5_chunks[] = {"a11"};
    struct sds_test a13_sds[] = {
	{0, a13_0_chunks, 2},
	{1, a13_1_chunks, 2},
	{2, a13_2_chunks, 2},
	{3, a13_3_chunks, 3},
	{4, a13_4_chunks, 2},
	{5, a13_5_chunks, 1}
    };
    struct sfd_sd_test a13 = {
	ACTION_ADD, 
	50,
	"a13",

	6,
	a13_sfds,
	6,
	a13_sds,
	6
    };
    
    struct sfds_test a14_sfds[] = {{0, 10}, {1, 30}, {2, 40}, {3, 50}, {4, 60}, {5, 100}};
    char *a14_0_chunks[] = {"a7", "a9"};
    char *a14_1_chunks[] = {"a2", "a3", "a14"};
    char *a14_2_chunks[] = {"a10", "a12"};
    char *a14_3_chunks[] = {"a5", "a8", "a13"};
    char *a14_4_chunks[] = {"a4", "a6"};
    char *a14_5_chunks[] = {"a11"};
    struct sds_test a14_sds[] = {
	{0, a14_0_chunks, 2},
	{1, a14_1_chunks, 3},
	{2, a14_2_chunks, 2},
	{3, a14_3_chunks, 3},
	{4, a14_4_chunks, 2},
	{5, a14_5_chunks, 1}
    };
    struct sfd_sd_test a14 = {
	ACTION_ADD, 
	30,
	"a14",

	6,
	a14_sfds,
	6,
	a14_sds,
	6
    };

    struct sfds_test a15_sfds[] = {{0, 10}, {1, 30}, {2, 40}, {3, 50}, {4, 60}, {5, 100}};
    char *a15_0_chunks[] = {"a7", "a9"};
    char *a15_1_chunks[] = {"a2", "a3", "a14"};
    char *a15_2_chunks[] = {"a10", "a12"};
    char *a15_3_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a15_4_chunks[] = {"a4", "a6"};
    char *a15_5_chunks[] = {"a11"};
    struct sds_test a15_sds[] = {
	{0, a15_0_chunks, 2},
	{1, a15_1_chunks, 3},
	{2, a15_2_chunks, 2},
	{3, a15_3_chunks, 4},
	{4, a15_4_chunks, 2},
	{5, a15_5_chunks, 1}
    };
    struct sfd_sd_test a15 = {
	ACTION_ADD, 
	50,
	"a15",

	6,
	a15_sfds,
	6,
	a15_sds,
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
    struct sds_test a16_sds[] = {
	{0, a16_0_chunks, 2},
	{1, a16_1_chunks, 1},
	{2, a16_2_chunks, 3},
	{3, a16_3_chunks, 2},
	{4, a16_4_chunks, 4},
	{5, a16_5_chunks, 2},
	{6, a16_6_chunks, 1}
    };
    struct sfd_sd_test a16 = {
	ACTION_ADD, 
	20,
	"a16",

	7,
	a16_sfds,
	7,
	a16_sds,
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
    struct sds_test a17_sds[] = {
	{0, a17_0_chunks, 2},
	{1, a17_1_chunks, 2},
	{2, a17_2_chunks, 3},
	{3, a17_3_chunks, 2},
	{4, a17_4_chunks, 4},
	{5, a17_5_chunks, 2},
	{6, a17_6_chunks, 1}
    };
    struct sfd_sd_test a17 = {
	ACTION_ADD, 
	20,
	"a17",

	7,
	a17_sfds,
	7,
	a17_sds,
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
    struct sds_test a18_sds[] = {
	{0, a18_0_chunks, 2},
	{1, a18_1_chunks, 2},
	{2, a18_2_chunks, 3},
	{3, a18_3_chunks, 2},
	{4, a18_4_chunks, 4},
	{5, a18_5_chunks, 2},
	{6, a18_6_chunks, 2}
    };
    struct sfd_sd_test a18 = {
	ACTION_ADD, 
	100,
	"a18",

	7,
	a18_sfds,
	7,
	a18_sds,
	7
    };

    struct sfd_sd_test a19 = {
	ACTION_ADD,
	19,
	"a19",

	7,
	a18_sfds,
	7,
	a18_sds,
	7
    };

    struct sfd_sd_test a20 = {
	ACTION_ADD,
	0,
	"a20",

	7,
	a18_sfds,
	7,
	a18_sds,
	7
    };

    struct sfd_sd_test a21 = {
	ACTION_ADD,
	8,
	"a21",

	7,
	a18_sfds,
	7,
	a18_sds,
	7
    };
    
    struct sfds_test a22_sfds[] = {{0, 10}, {1, 20}, {2, 30}, {3, 40}, {4, 50}, {5, 60}};
    char *a22_0_chunks[] = {"a7", "a9"};
    char *a22_1_chunks[] = {"a16", "a17"};
    char *a22_2_chunks[] = {"a2", "a3", "a14"};
    char *a22_3_chunks[] = {"a10", "a12"};
    char *a22_4_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a22_5_chunks[] = {"a4", "a6"};
    struct sds_test a22_sds[] = {
	{0, a22_0_chunks, 2},
	{1, a22_1_chunks, 2},
	{2, a22_2_chunks, 3},
	{3, a22_3_chunks, 2},
	{4, a22_4_chunks, 4},
	{5, a22_5_chunks, 2},
    };
    struct sfd_sd_test a22 = {
	ACTION_DELETE_INDEX,
	6,
	NULL,

	6,
	a22_sfds,
	6,
	a22_sds,
	6
    };

    struct sfd_sd_test a23 = {
	ACTION_DELETE_INDEX,
	10,
	NULL,

	6,
	a22_sfds,
	6,
	a22_sds,
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
    struct sds_test a24_sds[] = {
	{0, a24_0_chunks, 2},
	{1, a24_1_chunks, 2},
	{2, a24_2_chunks, 3},
	{3, a24_3_chunks, 1},
	{4, a24_4_chunks, 2},
	{5, a24_5_chunks, 4},
	{6, a24_6_chunks, 2},
    };
    struct sfd_sd_test a24 = {
	ACTION_ADD,
	35,
	"a24",

	7,
	a24_sfds,
	7,
	a24_sds,
	7
    };

    struct sfds_test a25_sfds[] = {{0, 20}, {1, 30}, {2, 35}, {3, 40}, {4, 50}, {5, 60}};
    char *a25_0_chunks[] = {"a16", "a17"};
    char *a25_1_chunks[] = {"a2", "a3", "a14"};
    char *a25_2_chunks[] = {"a24"};
    char *a25_3_chunks[] = {"a10", "a12"};
    char *a25_4_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a25_5_chunks[] = {"a4", "a6"};
    struct sds_test a25_sds[] = {
	{0, a25_0_chunks, 2},
	{1, a25_1_chunks, 3},
	{2, a25_2_chunks, 1},
	{3, a25_3_chunks, 2},
	{4, a25_4_chunks, 4},
	{5, a25_5_chunks, 2},
    };
    struct sfd_sd_test a25 = {
	ACTION_DELETE_INDEX,
	0,
	NULL,

	6,
	a25_sfds,
	6,
	a25_sds,
	6
    };

    struct sfds_test a26_sfds[] = {{0, 30}, {1, 35}, {2, 40}, {3, 50}, {4, 60}};
    char *a26_1_chunks[] = {"a2", "a3", "a14"};
    char *a26_2_chunks[] = {"a24"};
    char *a26_3_chunks[] = {"a10", "a12"};
    char *a26_4_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a26_5_chunks[] = {"a4", "a6"};
    struct sds_test a26_sds[] = {
	{0, a26_1_chunks, 3},
	{1, a26_2_chunks, 1},
	{2, a26_3_chunks, 2},
	{3, a26_4_chunks, 4},
	{4, a26_5_chunks, 2},
    };
    struct sfd_sd_test a26 = {
	ACTION_DELETE,
	20,
	NULL,

	5,
	a26_sfds,
	5,
        a26_sds,
	5	
    };

    struct sfds_test a27_sfds[] = {{0, 30}, {1, 35}, {2, 40}, {3, 45}, {4, 50}, {5, 60}};
    char *a27_1_chunks[] = {"a2", "a3", "a14"};
    char *a27_2_chunks[] = {"a24"};
    char *a27_3_chunks[] = {"a10", "a12"};
    char *a27_4_chunks[] = {"a27"};
    char *a27_5_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a27_6_chunks[] = {"a4", "a6"};
    struct sds_test a27_sds[] = {
	{0, a27_1_chunks, 3},
	{1, a27_2_chunks, 1},
	{2, a27_3_chunks, 2},
	{3, a27_4_chunks, 1},
	{4, a27_5_chunks, 4},
	{5, a27_6_chunks, 2},
    };
    struct sfd_sd_test a27 = {
	ACTION_ADD,
	45,
	"a27",

	6,
	a27_sfds,
	6,
        a27_sds,
	6	
    };

    struct sfds_test a28_sfds[] = {{0, 30}, {1, 35}, {2, 40}, {3, 45}, {4, 50}, {5, 60}};
    char *a28_1_chunks[] = {"a2", "a3", "a14"};
    char *a28_2_chunks[] = {"a24"};
    char *a28_3_chunks[] = {"a10", "a12"};
    char *a28_4_chunks[] = {"a27", "a28"};
    char *a28_5_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a28_6_chunks[] = {"a4", "a6"};
    struct sds_test a28_sds[] = {
	{0, a28_1_chunks, 3},
	{1, a28_2_chunks, 1},
	{2, a28_3_chunks, 2},
	{3, a28_4_chunks, 2},
	{4, a28_5_chunks, 4},
	{5, a28_6_chunks, 2},
    };
    struct sfd_sd_test a28 = {
	ACTION_ADD,
	45,
	"a28",

	6,
	a28_sfds,
	6,
        a28_sds,
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
    struct sds_test a29_sds[] = {
	{0, a29_0_chunks, 1},
	{1, a29_1_chunks, 3},
	{2, a29_2_chunks, 1},
	{3, a29_3_chunks, 2},
	{4, a29_4_chunks, 2},
	{5, a29_5_chunks, 4},
	{6, a29_6_chunks, 2},
    };
    struct sfd_sd_test a29 = {
	ACTION_ADD,
	15,
	"a29",

	7,
	a29_sfds,
	7,
        a29_sds,
	7	
    };

    struct sfd_sd_test a30 = {
	ACTION_ADD,
	16,
	"a30",

	7,
	a29_sfds,
	7,
	a29_sds,
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
    struct sds_test a31_sds[] = {
	{0, a31_0_chunks, 2},
	{1, a31_1_chunks, 3},
	{2, a31_2_chunks, 1},
	{3, a31_3_chunks, 2},
	{4, a31_4_chunks, 2},
	{5, a31_5_chunks, 4},
	{6, a31_6_chunks, 2},
    };
    struct sfd_sd_test a31 = {
	ACTION_ADD,
	15,
	"a31",

	7,
	a31_sfds,
	7,
        a31_sds,
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
    struct sds_test a32_sds[] = {
	{0, a32_0_chunks, 2},
	{1, a32_1_chunks, 3},
	{2, a32_2_chunks, 2},
	{3, a32_3_chunks, 2},
	{4, a32_4_chunks, 2},
	{5, a32_5_chunks, 4},
	{6, a32_6_chunks, 2},
    };
    struct sfd_sd_test a32 = {
	ACTION_ADD,
	35,
	"a32",

	7,
	a32_sfds,
	7,
        a32_sds,
	7	
    };

    struct sfds_test a33_sfds[] = {{0, 15}, {1, 30}, {2, 35}, {3, 45}, {4, 50}, {5, 60}};
    char *a33_0_chunks[] = {"a29", "a31"};
    char *a33_1_chunks[] = {"a2", "a3", "a14"};
    char *a33_2_chunks[] = {"a24", "a32"};
    char *a33_4_chunks[] = {"a27", "a28"};
    char *a33_5_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a33_6_chunks[] = {"a4", "a6"};
    struct sds_test a33_sds[] = {
	{0, a33_0_chunks, 2},
	{1, a33_1_chunks, 3},
	{2, a33_2_chunks, 2},
	{3, a33_4_chunks, 2},
	{4, a33_5_chunks, 4},
	{5, a33_6_chunks, 2},
    };
    struct sfd_sd_test a33 = {
	ACTION_DELETE,
	40,
	NULL,

	6,
	a33_sfds,
	6,
        a33_sds,
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
    struct sds_test a34_sds[] = {
	{0, a34_0_chunks, 2},
	{1, a34_1_chunks, 3},
	{2, a34_2_chunks, 2},
	{3, a34_3_chunks, 2},
	{4, a34_4_chunks, 0},
	{5, a34_5_chunks, 4},
	{6, a34_6_chunks, 2},
    };
    struct sfd_sd_test a34 = {
	ACTION_ADD,
	47,
	NULL,

	7,
	a34_sfds,
	7,
        a34_sds,
	7	
    };

    struct sfd_sd_test a35 = {
	ACTION_ADD,
	33,
	NULL,

	7,
	a34_sfds,
	7,
	a34_sds,
	7
    };

    struct sfd_sd_test a36 = {
	ACTION_ADD,
	47,
	NULL,

	7,
	a34_sfds,
	7,
	a34_sds,
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
    struct sds_test a37_sds[] = {
	{0, a37_0_chunks, 2},
	{1, a37_1_chunks, 3},
	{2, a37_2_chunks, 2},
	{3, a37_3_chunks, 2},
	{4, a37_4_chunks, 1},
	{5, a37_5_chunks, 4},
	{6, a37_6_chunks, 2},
    };
    struct sfd_sd_test a37 = {
	ACTION_ADD,
	47,
	"a37",

	7,
	a37_sfds,
	7,
        a37_sds,
	7	
    };

    struct sfd_sd_test a38 = {
	ACTION_ADD,
	47,
	NULL,

	7,
	a37_sfds,
	7,
	a37_sds,
	7
    };

    struct sfd_sd_test a39 = {
	ACTION_ADD,
	30,
	NULL,

	7,
	a37_sfds,
	7,
	a37_sds,
	7
    };

    struct sfds_test a40_sfds[] = {{0, 15}, {1, 30}, {2, 35}, {3, 45}, {4, 47}, {5, 50}, {6, 60}};
    char *a40_0_chunks[] = {"a29", "a31"};
    char *a40_1_chunks[] = {};
    char *a40_2_chunks[] = {"a24", "a32"};
    char *a40_3_chunks[] = {"a27", "a28"};
    char *a40_4_chunks[] = {"a37"};
    char *a40_5_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a40_6_chunks[] = {"a4", "a6"};
    struct sds_test a40_sds[] = {
	{0, a40_0_chunks, 2},
	{1, a40_1_chunks, 0},
	{2, a40_2_chunks, 2},
	{3, a40_3_chunks, 2},
	{4, a40_4_chunks, 1},
	{5, a40_5_chunks, 4},
	{6, a40_6_chunks, 2},
    };
    struct sfd_sd_test a40 = {
	ACTION_EMPTY_DCL,
	30,
	NULL,

	7,
	a40_sfds,
	7,
        a40_sds,
	7	
    };

    struct sfds_test a41_sfds[] = {{0, 30}, {1, 35}, {2, 45}, {3, 47}, {4, 50}, {5, 60}};
    char *a41_1_chunks[] = {};
    char *a41_2_chunks[] = {"a24", "a32"};
    char *a41_3_chunks[] = {"a27", "a28"};
    char *a41_4_chunks[] = {"a37"};
    char *a41_5_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a41_6_chunks[] = {"a4", "a6"};
    struct sds_test a41_sds[] = {
	{0, a41_1_chunks, 0},
	{1, a41_2_chunks, 2},
	{2, a41_3_chunks, 2},
	{3, a41_4_chunks, 1},
	{4, a41_5_chunks, 4},
	{5, a41_6_chunks, 2},
    };
    struct sfd_sd_test a41 = {
	ACTION_DELETE,
	15,
	NULL,

	6,
	a41_sfds,
	6,
        a41_sds,
	6	
    };

    struct sfds_test a42_sfds[] = {{0, 30}, {1, 35}, {2, 45}, {3, 47}, {4, 50}, {5, 60}};
    char *a42_1_chunks[] = {"a42"};
    char *a42_2_chunks[] = {"a24", "a32"};
    char *a42_3_chunks[] = {"a27", "a28"};
    char *a42_4_chunks[] = {"a37"};
    char *a42_5_chunks[] = {"a5", "a8", "a13", "a15"};
    char *a42_6_chunks[] = {"a4", "a6"};
    struct sds_test a42_sds[] = {
	{0, a42_1_chunks, 1},
	{1, a42_2_chunks, 2},
	{2, a42_3_chunks, 2},
	{3, a42_4_chunks, 1},
	{4, a42_5_chunks, 4},
	{5, a42_6_chunks, 2},
    };
    struct sfd_sd_test a42 = {
	ACTION_ADD,
	30,
	"a42",

	6,
	a42_sfds,
	6,
        a42_sds,
	6	
    };

    void print_test(struct sfd_sd_test *t)
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
	else if(t->action == ACTION_EMPTY_DCL) 
	    printf("empty_sd");
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

	printf("out_sds:\n ");
	if(t->out_sds != NULL) {
	    for(i = 0; i < t->out_sds_count; i++) {
		printf("\tpos: %d, length: %d, chunks: ", t->out_sds[i].pos, t->out_sds[i].length);
		size_t j;
		for(j = 0; j < t->out_sds[i].length; j++)
			printf("%.*s || ", DATA_CHUNK_SIZE, t->out_sds[i].chunks[j]);
		printf("\n");
	    }
	}
	else
	    printf(" (null)\n");
	printf("out_sds_count: %d\n", t->out_sds_count);
    }

    void print_sfd_sd(struct sfd_sd_storage *sfd_sd)
    {
	assert(sfd_sd != NULL);
	size_t i;
	printf("----------sfd_sd----------\n");
	printf("size: %d, count %d\n", sfd_sd->size, sfd_sd->count);
	if(sfd_sd->count > 0) {
	    printf("sfds: ");
	    for(i = 0; i < sfd_sd->size; i++) 
		printf("%d => %d, ", i, sfd_sd->sfds[i]);
	    printf("\nsds: ");
	    struct socket_data *sd;
	    struct data_chunk *dc;
	    for(i = 0; i < sfd_sd->size; i++) {
		sd = sfd_sd->sds[i];
		printf("\n\t%d => {", i);
		if(sd != NULL) {
		    dc = sd->first;
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

    bool check_result(struct sfd_sd_test *t, struct sfd_sd_storage *sfd_sd)
    {
	if(sfd_sd->count != t->out_count) {
	    printf("Test failed: count doesn't match: test->out_count = %d vs sfd_sd->count = %d\n", t->out_count, 
		    sfd_sd->count);
	    return false;
	}

	size_t i;
	size_t test_i;
	for(i = 0; i < t->out_sockfds_count; i++) {
	    test_i = t->out_sockfds[i].i; 
	    if(test_i < sfd_sd->count) {
		if(sfd_sd->sfds[test_i] != t->out_sockfds[i].sockfd) {
		    printf("Test failed: t->out_sockfds[%d] states that sfd_sd->sfds[%d] must "
			    "equal %d, but in fact it equals %d", i, test_i, t->out_sockfds[i].sockfd, sfd_sd->sfds[test_i]);
		    return false;
		}
	    }
	    else {
		printf("Test failed: sockfd test index is out of sockfds storage range: "
			"t->out_sockfds index i = %d. t->out_sockfds[i].i = %d >= sfd_sd->count = %d", i, test_i, 
			sfd_sd->count);
		return false;
	    }
	}

	struct socket_data *sd;
	struct data_chunk *dc;
	size_t actual_count;
	bool mismatch;
	for(i = 0; i < t->out_sds_count; i++) {
	    test_i = t->out_sds[i].pos;
	    if(test_i < sfd_sd->count) {
		sd = sfd_sd->sds[test_i];
		if(sd->length != t->out_sds[i].length) {
		    printf("Test failed: t->out_sds[%d].length = %d doesn't match"
			    "sfd_sd->sds[%d].length = %d", i, t->out_sds[i].length, test_i, sd->length);
		    return false;
		}
		actual_count = 0;
		dc = sd->first;
		mismatch = false;
		while((dc != NULL) && (!mismatch) && (actual_count < t->out_sds[i].length)) {
		    char *s = t->out_sds[i].chunks[actual_count];
		    if(memcmp(dc->chunk, s, strlen(s)) != 0) {
			mismatch = true;
			printf("Test failed: found a mismatch in datachunks test between t->out_sds[%d]."
				"chunks[%d] = \"%s\" and sd{%d} = \"%s\"", i, actual_count, s, 
				actual_count, dc->chunk);
		    }
		    else {
			actual_count++;
			dc = dc->next;
		    }
		}
		if(mismatch)
		    return false;
		if(actual_count < t->out_sds[i].length) {
		    printf("Test failed: t->out_sds[%d] constains more datachunks than the corresponding sd", i);
		    return false;
		}
	    }
	    else {
	    	printf("Test failed: sd test index is out of sds storage range: "
			"t->out_sds index i = %d. t->out_sds[i].pos = %d >= sfd_sd->count = %d", i, test_i, sfd_sd->count);
		return false;
	    }
	}

	return true;
    }


    struct sfd_sd_test *tests[] = {&a1, &a2, &a3, &a4, &a5, &a6, &a7, &a8, &a9, 
	&a10, &a11, &a12, &a13, &a14, &a15, &a16, &a17, &a18, &a19, &a20, &a21, 
	&a22, &a23, &a24, &a25, &a26, &a27, &a28, &a29, &a30, &a31, &a32, &a33, 
	&a34, &a35, &a36, &a37, &a38, &a39, &a40, &a41, &a42};

    size_t i;
    size_t nt = 42;
    struct sfd_sd_storage *sfd_sd = NULL;
    bool failure = false;

    for(i = 0; i < nt; i++) {	
	if(tests[i]->action == ACTION_CREATE) {
	    if(sfd_sd != NULL) {
		printf("Error: trying to create a non-empty sfd_sd on test %d\n", i);
	    }
	    else {	
		sfd_sd = sfd_sd_create(SFD_DCL_TEST_SIZE);
	    }
	}
	else if(tests[i]->action == ACTION_ADD) {
	    if(sfd_sd == NULL) {
		printf("Error: trying to add an element to a NULL sfd_sd");
		continue;
	    }
	    size_t sl = 0;
	    if(tests[i]->in_add_chunk != NULL)
		sl = strlen(tests[i]->in_add_chunk) + 1;
	    sfd_sd_add(sfd_sd, tests[i]->in_add_sockfd, tests[i]->in_add_chunk, sl, NULL);
	}
	else if(tests[i]->action == ACTION_DELETE_INDEX) {
	    if(sfd_sd == NULL) {
		printf("Error: trying to remove an element from a NULL sfd_sd");
		continue;
	    }
	    sfd_sd_delete_index(sfd_sd, tests[i]->in_add_sockfd);
	}
	else if(tests[i]->action == ACTION_DELETE) {
	    if(sfd_sd == NULL) {
		printf("Error: trying to remove an element from a NULL sfd_sd");
		continue;
	    }
	    sfd_sd_delete(sfd_sd, tests[i]->in_add_sockfd);
	}
	else if(tests[i]->action == ACTION_EMPTY_DCL) {
	    if(sfd_sd == NULL) {
		printf("Error: trying to EMPTY_DCL in a NULL sfd_sd");
		continue;
	    }
	    sfd_sd_empty_sd(sfd_sd, tests[i]->in_add_sockfd);
	}
	failure = !check_result(tests[i], sfd_sd);
	if(verbose) {
	    printf("\n\nTest #%d\n", i);
	    if(!failure) 
		printf("\n++ passed ++\n");
	    else  
		printf("\n##### FAILED #####\n");

	    print_test(tests[i]);
	    print_sfd_sd(sfd_sd);
	}
	if(failure && (!verbose)) {
	    printf("\n!!!!!!!!! One or more tests in do_sfd_sd_test failed. Call it with verbose = true for details\n");
	}
    }

    if(sfd_sd != NULL)
	sfd_sd_empty_and_kill(sfd_sd);
}
