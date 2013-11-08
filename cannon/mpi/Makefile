##### User configurable options #####

MPICC = mpicc
GCC = gcc

LIB_PATH = -L.
INCLUDE_DIR = .
LDFLAGS	= -lm

### End User configurable options ###

.PHONY: all clean

CFLAGS = -g -I. 
LIBS = $(LIB_PATH)
EXECS = cannon 
DEPS = *.h 

all: $(EXECS) matrix_gen

$(EXECS): cannon.o load.o MyMPI.o
	$(MPICC) $(CFLAGS) $^ -o $@ $(LIB_PATH) $(LDFLAGS)

%.o: %.c $(DEPS) 
	$(MPICC) $(CFLAGS) -c $< -o $@

matrix_gen: matrix_gen.o
	$(GCC) $(CFLAGS) $^ -o $@ $(LIB_PATH) $(LDFLAGS)

matrix_gen.o: matrix_gen.c
	$(GCC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *~ $(EXECS) matrix_gen *.dat *.res assignment*

