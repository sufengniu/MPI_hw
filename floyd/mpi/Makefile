CC=mpicc
CFLAGS=-g

all: build-parallel

build-parallel:
	make make-graph print-graph floyd-parallel

make-graph: make-graph.o graph.o
	$(CC) $(CFLAGS) $^ -o $@

make-graph.o: make-graph.c graph.h
	$(CC) $(CFLAGS) -c $< -o $@

graph.o: graph.c graph.h MyMPI.h
	$(CC) $(CFLAGS) -c $< -o $@

print-graph: print-graph.o graph.o
	$(CC) $(CFLAGS) $^ -o $@

print-graph.o: print-graph.c graph.h
	$(CC) $(CFLAGS) -c $< -o $@

floyd-parallel: floyd-parallel.o graph.o
	$(CC) $(CFLAGS) $^ -o $@

floyd-parallel.o: floyd-parallel.c graph.h MyMPI.h
	$(CC) $(CFLAGS) -c $< -o $@

MyMPI.o: MyMPI.c MyMPI.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o assignment* *~ make-graph print-graph floyd-serial floyd-parallel *.dat

