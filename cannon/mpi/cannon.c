#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mpi.h>

#include "MyMPI.h"
#include "load.h"

#define DIM	2	// define the cartesion topology dimension

int main(int argc, char *argv[])
{
	int i, j;
	int rank;	/* MPI_COMM_WORLD rank */
	int size;
	int id;		/* Cartesion rank */
	int n;		/* number of matrix element */
	double **A, **B, **C; 
	int root = 0;	/* master process to print info */	

	char *file[] = {"matrixA.dat", "matrixB.dat", "matrixC_mpi.res"};

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int msize = (int)floor(sqrt((double)size));
	
	if (size != msize * msize)
	{
		printf("the process number should be n^2 for square 2D mesh topology.\n");fflush(stdout);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	// define 2D mesh topology
	int nrow, ncol;	/* rows & columns, additional variable for higher dimension */
	int ndims;
	int dims[DIM];
	int periods[DIM];
	int coords[DIM];
	int reorder;

	MPI_Comm comm2D;

	ndims = DIM;
	nrow = msize;
	ncol = msize;
	dims[0] = nrow; dims[1] = ncol;
	periods[0] = 1; periods[1] = 1;	
	reorder = 1;

	MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, reorder, &comm2D);
	
	if(comm2D != MPI_COMM_NULL){
		MPI_Cart_coords(comm2D, rank, ndims, coords);
		MPI_Cart_rank(comm2D, coords, &id);
		printf("processor %s, coordinator[%d, %d], rank %d out of %d in 2D mesh\n", processor_name, coords[0], coords[1], id, size);fflush(stdout);
	}

	MPI_Barrier(MPI_COMM_WORLD);
	
	// matrix parallel read
	if (rank == root)
		printf("loading matrix input file...\n");

	pread_matrix(file[0], &n, &A, comm2D);
	pread_matrix(file[1], &n, &B, comm2D);	// B should be transposed	
	
	MPI_Barrier(comm2D);
	if (rank == root)
		printf("loading matrix file all done.\n");

	// MPI_Cart_shift();
	

	
	MPI_Comm_free(&comm2D);

	MPI_Finalize();

	return 0;
}
