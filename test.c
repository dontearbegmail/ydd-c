#include "datachunks.h"
#include <stdio.h>

void print_array(int *arr, size_t size)
{
    size_t i;
    for(i = 0; i < size; i++)
	printf("%d ", arr[i]);
}

bool are_matching(int *arr1, int *arr2, size_t size)
{
    bool ok = true;
    size_t i;
    for(i = 0; (i < size) && (ok); i++) {
	ok = arr1[i] == arr2[i];
    }
    return ok;
}

void sort_test() 
{
    int arr[10] = {110, 208, 218, 219, 290, 310, 311, 312, 314, 319};
    size_t count = 10;
    size_t pos, i;
    
    printf("\n\nArray: ");
    for(i = 0; i < count; i++) {
	printf("%d ", arr[i]);
    }
    printf("\nCount = %d\n", count);
    
    size_t ts = 6;
    int test[] = {219, 300, 313, 109, 311, 340};
    bool found;

    for(i = 0; i < ts; i++) {
	found = find_in_sorted_array(test[i], arr, 10, count, &pos);
	printf("Test: %d. Result: found = %s, pos = %d\n", test[i], (found ? "true" : "false"), pos);
    }
}

struct put_test {
    int in_v;
    int *in_arr;
    size_t in_size;
    size_t in_count;
    bool in_insert_dupl;

    int *ev_arr;
    size_t ev_count;
    size_t ev_position;
    int ev_return;
};

#define ARRLEN	9
int put_arr[ARRLEN] = {112, 144, 600, 600, 601, 710, 711, 713, 800}; // 9 items

struct put_test g = {
    710, put_arr, 9, 9, false,

    put_arr, 9, 0, -1 
};

int f_ev_arr[ARRLEN] = {112, 144, 600, 710, 601, 710, 711, 713, 800};
struct put_test f = {
    710, put_arr, 9, 3, true,

    f_ev_arr, 4, 3, 0
};

int e_ev_arr[ARRLEN] = {112, 144, 600, 610, 601, 710, 711, 713, 800};
struct put_test e = {
    610, put_arr, 9, 3, true,

    e_ev_arr, 4, 3, 0
};

int d_ev_arr[ARRLEN] = {112, 144, 600, 600, 601, 709, 710, 711, 713};
struct put_test d = {
    709, put_arr, 9, 8, true,

    d_ev_arr, 9, 5, 0
};

struct put_test c = {
    709, put_arr, 9, 9, true,

    put_arr, 9, 0, -1
};

int a_ev_arr[ARRLEN] = {112, 112, 144, 600, 600, 601, 710, 711, 713};
struct put_test a = {
    112, put_arr, 9, 8, true,

    a_ev_arr, 9, 0, 1
};

int b_ev_arr[ARRLEN] = {112, 144, 600, 600, 600, 601, 710, 713, 800};
struct put_test b = {
    600, put_arr, 9, 6, true,

    b_ev_arr, 7, 2, 1
};

size_t put_tests_num = 7; 
struct put_test *pt[] = {&a, &b, &c, &d, &e, &f, &g};

void put_to_array_test()
{
    size_t i, j, count, pos;
    int r;
    bool ok = true;
    int carr[ARRLEN];

    for(i = 0; i < put_tests_num; i++) {
	ok = true;
	for(j = 0; j < ARRLEN; j++) {
	    carr[j] = pt[i]->in_arr[j];
	}
	printf("Input array: ");
	print_array(pt[i]->in_arr, pt[i]->in_size);
	printf(" ||| input value: %d, size: %d, count: %d\n", pt[i]->in_v, pt[i]->in_size, pt[i]->in_count);
	printf("Expecting:   ");
	print_array(pt[i]->ev_arr, pt[i]->in_size);
	printf(" ||| count: %d, position: %d, return value: %d\n", pt[i]->ev_count, pt[i]->ev_position, pt[i]->ev_return);

	count = pt[i]->in_count;
	r = put_to_sorted_array(pt[i]->in_v, carr, pt[i]->in_size, &count, &pos, pt[i]->in_insert_dupl);

	if(!are_matching(carr, pt[i]->ev_arr, pt[i]->in_size)) {
	    printf("Result array doesn't match: ");
	    print_array(carr, pt[i]->in_size);
	    printf("\n");
	    ok = false;
	}
	if(pt[i]->ev_count != count) {
	    printf("Count doesn't match: %d vs %d\n", pt[i]->ev_count, count);
	    ok = false;
	}
	if(pt[i]->ev_position != pos) {
	    printf("Position doesn't match: %d vs %d\n", pt[i]->ev_position, pos);
	    ok = false;
	}
	if(pt[i]->ev_return != r) {
	    printf("Return value doesn't match: %d vs %d\n", pt[i]->ev_return, r);
	    ok = false;
	}
	printf("%s\n\n", ok ? "Test passed" : "Test failed");
    }
}


