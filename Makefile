.PHONY: all clean
CFLAGS=-g -Wall -ansi -D_GNU_SOURCE

all: ydd buddy

clean:
	rm -rf buddy ydd *.o

ydd: ydd.c datachunks.o comm.o general.o comm_ssl.o
	gcc $(CFLAGS) -o ydd ydd.c datachunks.o comm.o general.o comm_ssl.o -lssl

buddy: buddy.c datachunks.o comm.o general.o sfd_dcl_test.o dcl_test.o comm_ssl.o
	gcc $(CFLAGS) -o buddy buddy.c datachunks.o comm.o general.o comm_ssl.o sfd_dcl_test.o dcl_test.o -lssl

datachunks.o: datachunks.c
	gcc $(CFLAGS) -c -o datachunks.o datachunks.c

#ydapi_comm.o: ydapi_comm.c
#	gcc $(CFLAGS) -c -o ydapi_comm.o ydapi_comm.c

comm.o: comm.c
	gcc $(CFLAGS) -c -o comm.o comm.c

general.o: general.c
	gcc $(CFLAGS) -c -o general.o general.c

comm_ssl.o: comm_ssl.c
	gcc $(CFLAGS) -c -o comm_ssl.o comm_ssl.c

sfd_dcl_test.o: sfd_dcl_test.c
	gcc $(CFLAGS) -c -o sfd_dcl_test.o sfd_dcl_test.c

dcl_test.o: dcl_test.c
	gcc $(CFLAGS) -c -o dcl_test.o dcl_test.c
