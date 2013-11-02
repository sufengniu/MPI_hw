CC=gcc
CFLAGS=-g -Wall

LDFLAGS=-lrt
INCLUDES := -I.

all: matrix_gen matrix_multi

matrix_gen: matrix_gen.o
	$(CC) $(CFLAGS) $+ -o $@ $(LDFLAGS) 

matrix_gen.o: matrix_gen.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

matrix_multi: matrix_multi.o
	$(CC) $(CFLAGS) $+ -o $@ $(LDFLAGS)

matrix_multi.o: matrix_multi.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf *.o matrix_gen matrix_multi *.res
