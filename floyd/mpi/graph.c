#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "MyMPI.h"

int read_graph(char *file_name, int *n, int ***A)
{
	FILE *pFile;
	size_t result;	

	int *Astorage;
	int i, j;

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
	
	*A = (int **)calloc(*n, sizeof(int *));
	for (i = 0; i < *n; i++){
		if ((*(*A+i) = (int *)calloc(*n, sizeof(int))) == NULL){
			printf("Could not allocate enough memory for matrix A\n");
			exit(3);
		}	
	}	

	Astorage = (int *)malloc((*n)*(*n)*sizeof(int));
	
	result = fread(Astorage, sizeof(int), (*n)*(*n), pFile);
	if (result != (*n)*(*n)){
		fputs("Memory error\n", stderr);
		exit(2);
	}

	for (i = 0; i < *n; i++){
		for (j = 0; j < *n; j++){
			(*A)[i][j] = Astorage[j + i * (*n)];
		}
	}
	
	fclose(pFile);
	free(Astorage);	

	return 0;
}

// parallel read
#ifdef PARAL_MODE
int pread_graph(char *file_name, int *n, int ***A, MPI_Comm comm)
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
	
	*A = (int **)calloc(chunk_size, sizeof(int *));
	for (i = 0; i < chunk_size; i++){
		if ((*(*A+i) = (int *)calloc(*n, sizeof(int))) == NULL){
			printf("Could not allocate enough memory for matrix A\n");
			exit(3);
		}
	}
	Astorage = (int *)malloc(chunk_size*(*n)*sizeof(int));
	
	fseek(pFile, BLOCK_LOW(id, p, *n) * (*n) * sizeof(int) + sizeof(int), SEEK_SET);
	
	result = fread(Astorage, sizeof(int), chunk_size * (*n), pFile);
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
#endif

int write_graph(char *file_name, int n, int **A)
{
	printf("writing file to %s...\n", file_name);
	FILE *pFile;
	int i, j;

	int *Astorage;

	pFile = fopen(file_name, "wb");
	if (pFile == NULL){
		fputs("File error\n", stderr);
		exit(4);
	}	

	fwrite(&n, sizeof(int), 1, pFile);

	Astorage = (int *)malloc(n*n*sizeof(int));
	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){
			Astorage[j + i*n] = A[i][j];
		}
	}
		
	fwrite(Astorage, sizeof(int), n*n, pFile);

	fclose(pFile);

	free(Astorage);
	printf("writing file done\n");
	return 0;
}

#ifdef PARAL_MODE
int pwrite_graph(char *file_name, int n, int **A, int chunk_size, MPI_Comm comm)
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
#endif

int print_graph(int n, int **A)
{
	int i, j;
	printf("print out the %d by %d graph matrix.\n", n, n);
	printf("====================================\n");
		
	for (i = 0; i < n; i++)
	{
		printf("row %d: ", i);
		for (j = 0; j < n; j++){
			if (j == n-1)
				printf("%d\n", A[i][j]);
			else
				printf("%d\t", A[i][j]);
		}
	}	
	return 0;
}
