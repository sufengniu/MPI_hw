##### User configurable options #####

CC = mpicc

LIB_PATH = -L.
INCLUDE_DIR = .
LDFLAGS	= -lm

### End User configurable options ###

.PHONY: all clean

CFLAGS = -g -I. 
LIBS = $(LIB_PATH)
EXECS = cannon 
DEPS = *.h 

all: $(EXECS)

$(EXECS): cannon.o load.o MyMPI.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIB_PATH) $(LDFLAGS)

%.o: %.c $(DEPS) 
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *~ $(EXECS)

