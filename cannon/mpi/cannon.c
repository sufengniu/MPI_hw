#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <mpi.h>

#include "MyMPI.h"
#include "load.h"

#define DIM	2	// define the cartesion topology dimension

int main(int argc, char *argv[])
{
	/* define matrix variables */
	int i, j, k, l;
	int rank;	/* MPI_COMM_WORLD rank */
	int size;	/* MPI_COMM_WORLD process number */
	int id;		/* Cartesion rank */
	int m, n;	/* m: matrix array rows, n: matrix array cols */
	double **A, **B; 	
	double *Astorage, *Bstorage, *Cstorage, *Cbuff, *C;
	int chunk_size = 0;	/* checkerboard total size */
	int row;	/* sub matrix row size */
	int col;	/* sub matrix col size */
	int root = 0;	/* master process to print info */	
	int source, dest;	
	int global_i=0;	/* matrix row index for checkerboard gather function */
	int global_j=0;	/* matrix col index for checkerboard gather function */
	int offset = 0;	/* offset to transfer checkerboard to the correct order */
	double t1, t2;

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

	int direction;	/* determine which dimension perform shift, dir=0: shift between row, dir=1: shift between col */
	int disp;	/* disp>0: upward shift, disp<0: downward shift */

	MPI_Comm comm2D;
	MPI_Status status;

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

	chunk_size = pread_matrix(file[0], &n, &row, &col, msize, &A, &Astorage, comm2D);
	chunk_size = pread_matrix(file[1], &n, &row, &col, msize, &B, &Bstorage, comm2D);

	MPI_Barrier(MPI_COMM_WORLD);	
	if (rank == root)
		printf("loading matrix file all done.\n");

	/* memor allocate for process buffer and output matrix */
	Cbuff = (double *)malloc(chunk_size*sizeof(double));
	Cstorage = (double *)malloc(n*n*sizeof(double));
	C = (double *)malloc(n*n*sizeof(double));

	for (i = 0; i < chunk_size; i++){
		Cbuff[i] = 0;
	}

	printf("rank: %d, coords[0]: %d, coords[1]: %d\n", rank, coords[0], coords[1]);

	t1 = MPI_Wtime();
	/* alignment */
	// up shift 
	direction = 0;
	disp = -coords[1];
	MPI_Cart_shift(comm2D, direction, disp, &source, &dest);
	MPI_Sendrecv_replace(Bstorage, chunk_size, MPI_DOUBLE, dest, 0,
			source, 0, comm2D, &status);

	// left shift 
	direction = 1;
	disp = -coords[0];
	MPI_Cart_shift(comm2D, direction, disp, &source, &dest);
	MPI_Sendrecv_replace(Astorage, chunk_size, MPI_DOUBLE, dest, 0,
			source, 0, comm2D, &status);

	MPI_Barrier(comm2D);

	for (k = 0; k < msize; k++){

		// calculation
		for (i = 0; i < row; i++){
			for (j = 0; j < col; j++){
				for (l = 0; l < col; l++){
					Cbuff[j + i*col] += Astorage[l + i*col] * Bstorage[j + l*col];
				}
			} 
		}

		// left shift 
		direction = 0;
		disp = -1;
		MPI_Cart_shift(comm2D, direction, disp, &source, &dest); 
		MPI_Sendrecv_replace(Bstorage, chunk_size, MPI_DOUBLE, dest, 0,
				source, 0, comm2D, &status);

		// up shift 
		direction = 1;
		disp = -1;
		MPI_Cart_shift(comm2D, direction, disp, &source, &dest);
		MPI_Sendrecv_replace(Astorage, chunk_size, MPI_DOUBLE, dest, 0,
				source, 0, comm2D, &status);
	}	

	MPI_Allgather(Cbuff, chunk_size, MPI_DOUBLE, Cstorage, chunk_size, MPI_DOUBLE, comm2D);
	
	t2 = MPI_Wtime();
	printf("process %d, measured results: %f second\n", rank, t2-t1);fflush(out);
	
	// rearrange the matrix C in a proper order
	if (rank == root){
		for (i = 0; i < size; i++){
			for (j = 0; j < row*col; j++){
				global_i = (int)(j/col) + row * (int)(i/msize);
				global_j = j%col + (i%msize) * col;

				offset = global_i * n + global_j; 
				C[offset] = Cstorage[j+i*row*col];

			}
		}

		fprintf_matrix(file[2], n, C);
		printf("process %d writing file done\n", rank);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	// mem clean up
	for (i = 0; i < row; i++){
		free(A[i]);
	}
	free(A);
	for (i = 0; i < row; i++){
		free(B[i]);	
	}
	free(B);

	free(C);

	free(Cbuff);

	free(Astorage);
	free(Bstorage);
	free(Cstorage);

	MPI_Finalize();

	return 0;
}
