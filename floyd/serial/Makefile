CC=gcc
CFLAGS=-g

all: build-serial

build-serial:
	make make-graph print-graph floyd-serial

make-graph: make-graph.o graph.o
	$(CC) $(CFLAGS) $^ -o $@

make-graph.o: make-graph.c graph.h
	$(CC) $(CFLAGS) -c $< -o $@

graph.o: graph.c graph.h
	$(CC) $(CFLAGS) -c $< -o $@

print-graph: print-graph.o graph.o
	$(CC) $(CFLAGS) $^ -o $@

print-graph.o: print-graph.c graph.h
	$(CC) $(CFLAGS) -c $< -o $@

floyd-serial: floyd-serial.o graph.o
	$(CC) $(CFLAGS) $^ -o $@ -lrt

floyd-serial.o: floyd-serial.c graph.h
	$(CC) $(CFLAGS) -c $< -o $@

MyMPI.o: MyMPI.c MyMPI.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o assignment* make-graph print-graph floyd-serial floyd-parallel *.dat 

