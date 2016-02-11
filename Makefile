.PHONY: all clean

all: ydd buddy

clean:
	rm -rf buddy ydd *.o

ydd: ydd.c httpchunks.o ydapi_comm.o comm.o general.o
	gcc -g -Wall -o ydd ydd.c httpchunks.o ydapi_comm.o comm.o general.o -lssl

buddy: buddy.c httpchunks.o ydapi_comm.o comm.o general.o
	gcc -g -Wall -o buddy buddy.c httpchunks.o ydapi_comm.o comm.o general.o -lssl

httpchunks.o: httpchunks.c
	gcc -g -Wall -c -o httpchunks.o httpchunks.c

ydapi_comm.o: ydapi_comm.c
	gcc -g -Wall -c -o ydapi_comm.o ydapi_comm.c

comm.o: comm.c
	gcc -g -Wall -c -o comm.o comm.c

general.o: general.c
	gcc -g -Wall -c -o general.o general.c
