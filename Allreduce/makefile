CC=mpicc
CFLAGES=-g

all: main

main: main.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o main example.o*


