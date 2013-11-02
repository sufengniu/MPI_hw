#include <stdlib.h>
#include <stdio.h>

#include <mpi.h>

#include "MyMPI.h"
#include "graph.h"

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

int main(int argc, char **argv)
{
	int rank;
	int size;
	int i, j, k;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	printf("processor %s, rank %d out of %d processors\n",
			processor_name, rank, size);fflush(stdout);

	MPI_Barrier(MPI_COMM_WORLD);

	int **A;
	int **B;
	int n;
	int offset = 0;
	int chunk_size;
	int *tmp;
	int root;		// root id
	double t1, t2;

	// parallel read
	chunk_size = pread_graph(argv[1], &n, &A, MPI_COMM_WORLD);

	// testing
	printf("chunk size is %d, rank: %d\n", chunk_size, rank);

	tmp = (int *)malloc(n * sizeof(int));
	B = (int **)calloc(chunk_size, sizeof(int *));
	for (i = 0; i < chunk_size; i++){
		if ((*(B+i) = (int *)calloc(n, sizeof(int))) == NULL){
			printf("Could not allocate enough memory for matrix B\n");
			exit(3);
		}
	}
		
	for (i = 0; i < chunk_size; i++)
		for(j = 0; j < n; j++)
			B[i][j]=A[i][j];

	MPI_Barrier(MPI_COMM_WORLD);

	t1 = MPI_Wtime();
	for (k = 0; k < n; k++){
	
		root = BLOCK_OWNER(k, size, n);
		
		if (rank == root){
			offset = k - BLOCK_LOW(rank, size, n);

			for (j = 0; j < n; j++)
				tmp[j] = B[offset][j];
		}
		
		MPI_Bcast(tmp, n, MPI_INT, root, MPI_COMM_WORLD);
		
		for (i = 0; i < chunk_size; i++){
			for (j = 0; j < n; j++){
					
				if ((B[i][k] == -1) || (tmp[j] == -1))
					B[i][j] = B[i][j];
				else if (B[i][j] == -1)
					B[i][j] = B[i][k] + tmp[j];
				else
					B[i][j] = min(B[i][j], B[i][k] + tmp[j]);
			}
		}
		
	}
	t2 = MPI_Wtime();
		
	printf("process %d: measured results: %f seconds\n", rank, t2-t1); fflush(stdout);

	pwrite_graph(argv[2], n, B, chunk_size, MPI_COMM_WORLD);

	for (i = 0; i < chunk_size; i++)
	{
		free(A[i]);
	}
	free(A);

	MPI_Finalize();
	
	return 0;
}
