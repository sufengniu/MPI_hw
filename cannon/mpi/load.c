#include <stdio.h>
#include <stdlib.h>

#include "load.h"
#include "MyMPI.h"

int pread_matrix(char *file_name, int *n, int mszie, double ***A, MPI_Comm comm)
{
	int id;		// Process rank
	int p;		// Process number
	int i, j;
	FILE *pFile;
	size_t result;
	int *Astorage;
		
	MPI_Comm_size(comm, &p);
	MPI_Comm_rank(comm, &id);
	
	printf("process %d reading file...\n", id);	
	pFile = fopen(file_name, "rb");
	if (pFile == NULL)
	{
		fputs("File error\n", stderr);
		exit(1);
	}
	
	rewind(pFile);
	
	result = fread(n, sizeof(int), 1, pFile);
	if (result != 1){
		fputs("Memory error\n", stderr);
		exit(2);
	}

	// partitioning
	int chunk_size;
	
	chunk_size = BLOCK_SIZE(id, p, *n);
	
	*A = (double **)calloc(chunk_size, sizeof(void *));
	for (i = 0; i < chunk_size; i++){
		if ((*(*A+i) = (double *)calloc(*n, sizeof(double))) == NULL){
			printf("Could not allocate enough memory for matrix A\n");
			exit(3);
		}
	}
	Astorage = (double *)malloc(chunk_size*(*n)*sizeof(double));
	
	fseek(pFile, BLOCK_LOW(id, p, *n) * (*n) * sizeof(double) + sizeof(int), SEEK_SET);
	
	result = fread(Astorage, sizeof(double), chunk_size * (*n), pFile);
	if (result != chunk_size * (*n)){
		fputs("Memory error\n", stderr);
		exit(2);
	}	

	for (i = 0; i < chunk_size; i++){
		for (j = 0; j < *n; j++){
			(*A)[i][j] = Astorage[j + i * (*n)];
		}
	}	   

	fclose(pFile);	
	free(Astorage);
	printf("process %d reading file done\n", id);
	
	return chunk_size;
}

int pfprintf_matrix(char *file_name, int n, double **A, int chunk_size, MPI_Comm comm)
{
	int id;         // Process rank                                
	int p;          // Process number                              
	int i, j;                                                      
	FILE *pFile;                                                   
	size_t result;                                                 
	int *Astorage;                                                 

	MPI_Comm_size(comm, &p);                                       
	MPI_Comm_rank(comm, &id);                                      

	pFile = fopen(file_name, "wb");
	if (pFile == NULL)
	{
		fputs("File error\n", stderr);
		exit(1);
	}

	rewind(pFile);

	if (id == 0)
		fwrite(&n, sizeof(int), 1, pFile);

	MPI_Barrier(comm);
		
	Astorage = (int *)malloc(chunk_size*n*sizeof(int));
	for (i = 0; i < chunk_size; i++){
		for (j = 0; j < n; j++){
			Astorage[j+i*n] = A[i][j];
		}
	}	
	
	fseek(pFile, BLOCK_LOW(id, p, n) * n * sizeof(int) + sizeof(int), SEEK_SET);

	fwrite(Astorage, sizeof(int), chunk_size * n, pFile);
	
	fclose(pFile);
	free(Astorage);	
	
	printf("process %d writing file...\n", id);	
	return 0;
}

