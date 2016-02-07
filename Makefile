.PHONY: all clean

all: ydd

clean:
	rm -rf ydd *.o

#ydd: ydd.c httpchunks.c ydapi_comm.c comm.c
#	gcc -g -Wall ydd.c httpchunks.c ydapi_comm.c comm.c -lssl

ydd: ydd.c httpchunks.o ydapi_comm.o comm.o
	gcc -g -Wall -o ydd ydd.c httpchunks.o ydapi_comm.o comm.o -lssl

httpchunks.o: httpchunks.c
	gcc -g -Wall -c -o httpchunks.o httpchunks.c

ydapi_comm.o: ydapi_comm.c
	gcc -g -Wall -c -o ydapi_comm.o ydapi_comm.c

comm.o: comm.c
	gcc -g -Wall -c -o comm.o comm.c
