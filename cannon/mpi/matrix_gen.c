#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define DEFAULT_SIZE	300
#define DEFAULT_SCALE	100

void matrix_write(char *file_name, int n, double **A);

int main(int argc, char *argv[])
{
	int n = 0;
	int r = 0;
	int i, j;

	// matrix A and matrix B
	double **A, **B;

	char *output_file[] = {"matrixA.dat", "matrixB.dat"};

	int index;
	int opt;
	opterr = 0;

	if (argc <= 1){
		printf("no input arguments, using default value\n");
		n = DEFAULT_SIZE;
		r = DEFAULT_SCALE;
		printf("default matrix size n: %d, defaule matrix element scale: %d\n", n, r);
	} 
	else
	{
		while((opt = getopt(argc, argv, "n:r:")) != -1)
			switch (opt)
			{
				case 'n':
					n = atoi(optarg);
					break;
				case 'r':
					r = atoi(optarg);
					break;
				case '?':
					if ((optopt == 'n') || (optopt == 'r'))
						fprintf(stderr, "Option -%c requires an argument.\n", optopt);
					else if (isprint(optopt))
						fprintf(stderr, "Unknown option '-%c'.\n", optopt);
					else
						fprintf(stderr, "Unknown character '\\x%x'.\n", optopt);
					return 1;
				default:
					abort();

			}
	}

	for(index = optind; index < argc; index++)
		printf("Non-option argument %s\n", argv[index]);

	A = (double **)malloc(n * sizeof(void *));
	for (i = 0; i < n; i++){
		if ((*(A+i) = (double *)malloc(n * sizeof(double))) == NULL){
			fprintf(stderr, "Could not allocate enough memory for matrix A\n");
			exit(0);
		}
	}
	B = (double **)malloc(n * sizeof(void *));
	for (i = 0; i < n; i++){
		if ((*(B+i) = (double *)malloc(n * sizeof(double))) == NULL){
			fprintf(stderr, "Could not allocate enough memory for matrix B\n");
			exit(0);
		}
	}

	printf("create random matrix...\n");

	int seed = (int)time(0);
	double random;

	printf("@seed %d\n", seed);	
	srand(seed);	

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			random = (double) rand() / (RAND_MAX);
			A[i][j] = r * random;
			random = (double) rand() / (RAND_MAX);
			B[i][j] = r * random;
		}
	}

	matrix_write(output_file[0], n, A);
	matrix_write(output_file[1], n, B);

	for (i = 0; i < n; i++){
		free(A[i]);
		free(B[i]);
	}
	free(A);
	free(B);

	return 0;
}

void matrix_write(char *file_name, int n, double **A)
{
	printf("writing file to %s...\n", file_name);
	FILE *pFile;
	int i, j;
	double *Astorage;

	pFile = fopen(file_name, "wb");
	if (pFile == NULL){
		fputs("File error\n", stderr);
		exit(4);
	}

	fwrite(&n, sizeof(int), 1, pFile);
	fwrite(&n, sizeof(int), 1, pFile);
	
	Astorage = (double *)malloc(n*n*sizeof(double));
	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++){
			Astorage[j + i*n] = A[i][j];
		}
	}

	fwrite(Astorage, sizeof(double), n*n, pFile);

	fclose(pFile);
	free(Astorage);
	printf("writing file done.\n");

}

