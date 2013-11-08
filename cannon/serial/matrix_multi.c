#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BILLION 1000000000L

// load generated matrix data(.dat file) into memory
void matrix_load(char *file_name, int *n, double ***A);

// file printf results to .res file for comparision
void matrix_print(char *file_name, int n, double **C);

int main(int argc, char *argv[])
{
	int i, j, k;

	char *file[] = {"matrixA.dat", "matrixB.dat", "matrixC_serial.res"};
	double **A, **B, **C;
	int n;
	
	struct timespec start, stop;
	double accum;
	
	printf("loading input matrix file...\n");	
	matrix_load(file[0], &n, &A);
	matrix_load(file[1], &n, &B);
	printf("matrix loading done\n");
	
	printf("matrix size is %d by %d\n", n, n);
	C = (double **)malloc(n * sizeof(void *));
	for (i = 0; i < n; i++){
		if ((*(C+i) = (double *)calloc(n, sizeof(double))) == NULL){
			fprintf(stderr, "Could not allocate enough memory for matrix C!\n");
			exit(3);
		}
	}
	
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);
	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){
			C[i][j] = 0;
			for(k = 0; k < n; k++){
				C[i][j] = C[i][j] + A[i][k] * B[k][j];
			}
		}
	}
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stop);
	accum = (stop.tv_sec - start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec)/(double)BILLION;
	printf("computation done in %lf second\n", accum);

	printf("print result to file %s...\n", file[2]);
	matrix_print(file[2], n, C);
	printf("file printf done.\n");	

/*
	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){
			printf("%6.3f ", C[i][j]);
		}
		putchar('\n');
	}
*/	
	for (i = 0; i < n; i++){
		free(A[i]);
		free(B[i]);
		free(C[i]);
	}
	free(A);
	free(B);
	free(C);
	
	return 0;
}

void matrix_load(char *file_name, int *n, double ***A)
{
	FILE *pFile;
	size_t result;

	double *Astorage;
	int i, j;

	pFile = fopen(file_name, "rb");
	if (pFile == NULL) {
		fputs("FIle error\n", stderr);
		exit(1);
	}

	rewind(pFile);

	result = fread(n, sizeof(int), 1, pFile);
	if (result != 1){
		fputs("Memory error\n", stderr);
		exit(2);
	}

	result = fread(n, sizeof(int), 1, pFile);
	if (result != 1){
		fputs("Memory error\n", stderr);
		exit(2);
	}

	*A = (double **)calloc(*n, sizeof(double *));
	for (i = 0; i < *n; i++){
		if ((*(*A+i) = (double *)calloc(*n, sizeof(double))) == NULL){
			printf("Could not allocate enough memory for matrix\n");
			exit(3);
		}
	}
	Astorage = (double *)malloc((*n)*(*n)*sizeof(double));

	result = fread(Astorage, sizeof(double), (*n)*(*n), pFile);
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
}

void matrix_print(char *file_name, int n, double **C)
{
	int i, j;
	printf("printf to file %s...\n", file_name);
	FILE *pFile;
	
	double *Cstorage;

	Cstorage= (double *)malloc(n*n*sizeof(double));

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			Cstorage[j + i*n] = C[i][j];
		}
	}

	pFile = fopen(file_name, "w");
	
	fprintf(pFile, "%d\n", n);
	
	for (i = 0; i < n*n; i++){
		fprintf(pFile, "%f\n", Cstorage[i]);
	}
	
	fclose(pFile);
	free(Cstorage);
	printf("printf file done.\n");
}
