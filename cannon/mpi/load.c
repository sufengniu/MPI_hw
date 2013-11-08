#include <stdio.h>
#include <stdlib.h>

#include <mpi.h>
#include "load.h"
#include "MyMPI.h"

/* parallel read
 * attention: the matrix must be square matrix,
 * the number of matrix row and column must be 
 * square of n. if not, the program cannot 
 * partition it to get segemantation fault
 */
int pread_matrix(char *file_name, int *n, int *srow, int *scol, int msize, double ***A, double **Astorage, MPI_Comm comm)
{
	int id;		// Process rank
	int p;		// Process number
	int i, j;
	FILE *pFile;
	size_t result;
	
	int m; // no need to output m

	MPI_Comm_size(comm, &p);
	MPI_Comm_rank(comm, &id);

	printf("process %d reading file %s...\n", id, file_name);	

	pFile = fopen(file_name, "rb");
	if (pFile == NULL)
	{
		fputs("File error\n", stderr);
		exit(1);
	}

	rewind(pFile);

	/* m and n must be equal */
	result = fread(&m, sizeof(int), 1, pFile);
	if (result != 1){
		fputs("Memory error\n", stderr);
		exit(2);
	}

	result = fread(n, sizeof(int), 1, pFile);
	if (result != 1){
		fputs("Memory error\n", stderr);
		exit(2);
	}

	if (m != *n){
		fputs("m is not equal to n, the matrix cannot be partitioned!\n", stderr);
		exit(8);
	}
	
	// partitioning
	int chunk_size;
	int sub_row = 0;	/* sub matrix row number */
	int sub_col = 0;	/* sub matrix col number */
	int offset = 0;		/* offset for process start point */

	chunk_size = BLOCK_SIZE(id, p, (*n)*(*n));

	sub_row = (int)(*n / msize);
	sub_col = sub_row;

	*srow = sub_row;
	*scol = sub_col;

	if(chunk_size != (sub_row * sub_col)){
		printf("the process number or matrix size is too hard to be partitioned!\n");
		exit(10);
	}	

	*A = (double **)calloc(sub_row, sizeof(void *));
	for (i = 0; i < sub_row; i++){
		if ((*(*A+i) = (double *)calloc(sub_col, sizeof(double))) == NULL){

			printf("Could not allocate enough memory for input matrix\n");
			exit(3);
		}
	}
	*Astorage = (double *)malloc(chunk_size*sizeof(double));
	
	rewind(pFile);	
	for (i = 0; i < sub_row; i++){	

		offset = (int)(id/msize) * chunk_size * msize + (id%msize) * sub_col + i * (*n);
		fseek(pFile, offset*sizeof(double) + 2*sizeof(int), SEEK_SET);

		result = fread((*Astorage)+i*sub_col, sizeof(double), sub_col, pFile);
		if (result != sub_col){
			fputs("Memory error\n", stderr);
			exit(2);
		}	
	}

	for (i = 0; i < sub_row; i++){
		for (j = 0; j < sub_col; j++){
			(*A)[i][j] = (*Astorage)[j + i * sub_col];
		}
	}	   

	fclose(pFile);	
	printf("process %d reading file done\n", id);

	return chunk_size;
}

int fprintf_matrix(char *file_name, int n, double *Cstorage)
{
	int i, j;                                                      
	FILE *pFile;                                                   
	size_t result;                                                 

	pFile = fopen(file_name, "w");
	if (pFile == NULL)
	{
		fputs("File error\n", stderr);
		exit(1);
	}	

	fprintf(pFile, "%d\n", n);
		
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){		
			fprintf(pFile, "%f\n", Cstorage[j+i*n]);	
		}
	}

	fclose(pFile);

	return 0;
}

