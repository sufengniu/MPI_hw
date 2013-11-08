

all: build-serial build-parallel

build-serial:
	(cd serial && make)

build-parallel:
	(cd mpi && make)

clean:
	(cd mpi && make clean)
	(cd serial && make clean)

