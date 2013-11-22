CC=gcc
CFLAGS=-g

all: matrix_gen print-graph

print-graph: print-graph.o
	$(CC) $(CFLAGS) $^ -o $@

print-graph.o: print-graph.c
	$(CC) $(CFLAGS) -c $< -o $@

matrix_gen: matrix_gen.o
	$(CC) $(CFLAGS) $^ -o $@

matrix_gen.o: matrix_gen.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o matrix_gen print-graph *.dat
