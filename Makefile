.PHONY: all clean

all: ydd buddy

clean:
	rm -rf ydd *.o

#ydd: ydd.c httpchunks.c ydapi_comm.c comm.c
#	gcc -std=c99 -g -Wall ydd.c httpchunks.c ydapi_comm.c comm.c -lssl

ydd: ydd.c httpchunks.o ydapi_comm.o comm.o common.o
	gcc -g -Wall -o ydd ydd.c httpchunks.o ydapi_comm.o comm.o common.o -lssl

buddy: buddy.c httpchunks.o ydapi_comm.o comm.o common.o
	gcc -g -Wall -o buddy buddy.c httpchunks.o ydapi_comm.o comm.o common.o -lssl

httpchunks.o: httpchunks.c
	gcc -g -Wall -c -o httpchunks.o httpchunks.c

ydapi_comm.o: ydapi_comm.c
	gcc -g -Wall -c -o ydapi_comm.o ydapi_comm.c

comm.o: comm.c
	gcc -g -Wall -c -o comm.o comm.c

common.o: common.c
	gcc -g -Wall -c -o common.o common.c
