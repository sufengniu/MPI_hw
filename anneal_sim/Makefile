MPICC=mpicc
GCC=gcc

CLFAGS=-g
LDFLAGS=-L.

# Paths
BIN_PATH = bin/
SRC_PATH = src/

.PHONY: all clean tar


all:

$(BIN_PATH)%.o: $(SRC_PATH)%.c
	$(MPICC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o *~ 

tar:
	


