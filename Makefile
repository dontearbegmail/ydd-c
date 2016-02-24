.PHONY: all clean

all: ydd buddy

clean:
	rm -rf buddy ydd *.o

ydd: ydd.c datachunks.o ydapi_comm.o comm.o general.o
	gcc -g -Wall -o ydd ydd.c datachunks.o ydapi_comm.o comm.o general.o -lssl

buddy: buddy.c datachunks.o ydapi_comm.o comm.o general.o test.o sfd_dcl_test.o
	gcc -g -Wall -o buddy buddy.c datachunks.o ydapi_comm.o comm.o general.o test.o sfd_dcl_test.o -lssl

datachunks.o: datachunks.c
	gcc -g -Wall -c -o datachunks.o datachunks.c

ydapi_comm.o: ydapi_comm.c
	gcc -g -Wall -c -o ydapi_comm.o ydapi_comm.c

comm.o: comm.c
	gcc -g -Wall -c -o comm.o comm.c

general.o: general.c
	gcc -g -Wall -c -o general.o general.c

test.o: test.c
	gcc -g -Wall -c -o test.o test.c

sfd_dcl_test.o: sfd_dcl_test.c
	gcc -g -Wall -c -o sfd_dcl_test.o sfd_dcl_test.c
