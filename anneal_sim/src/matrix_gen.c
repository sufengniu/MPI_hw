#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define DEFAULT_SIZE	30
#define DEFAULT_SCALE	10

void matrix_write(char *file_name, int n, int **D);

int main(int argc, char *argv[])
{
	int n = 0;
	int r = 0;
	int i, j;

	// matrix D
	int **D;

	char *output_file[] = {"matrixD.dat"};	// dislike matrix

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

	D = (int **)malloc(n * sizeof(void *));
	for (i = 0; i < n; i++){
		if ((*(D+i) = (int *)malloc(n * sizeof(int))) == NULL){
			fprintf(stderr, "Could not allocate enough memory for matrix D\n");
			exit(0);
		}
	}

	printf("create dislike matrix...\n");

	int seed = (int)time(0);
	float random;

	printf("@seed %d\n", seed);	
	srand(seed);	

	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {

			if (i == j)
			{
				D[i][j] = 0;
			}
			else
			{
				random = (float) rand() / (RAND_MAX);
				D[i][j] = (int) (r-1) * random + 1;
			}	
		}
	}

	matrix_write(output_file[0], n, D);	

	for (i = 0; i < n; i++){
		free(D[i]);	
	}
	free(D);

	return 0;
}

void matrix_write(char *file_name, int n, int **A)
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
	printf("writing file done.\n");

}

