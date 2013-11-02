#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "graph.h"

//#define TEST	 

#ifdef TEST
#define N 5
#endif

int main(int argc, char **argv)
{
	
	int n = 0;		// nodes number
	int r = 0;		// edge weights
	int p = 0;		//
	char *output_file = NULL;
	
	int index;
	int opt;
	opterr = 0;
	
	while((opt = getopt(argc, argv, "n:r:p:o:")) != -1)
		switch (opt)
		{
			case 'n':
				n = atoi(optarg);
				break;
			case 'r':
				r = atoi(optarg);
				break;
			case 'p':
				p = atoi(optarg);
				break;
			case 'o':
				output_file = optarg;
				break;
			case '?':
				if ((optopt == 'n') || (optopt == 'r') || (optopt == 'p') || (optopt == 'o'))
					fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint(optopt))
					fprintf(stderr, "Unknown option '-%c'.\n", optopt);
				else
					fprintf(stderr, "Unknown character '\\x%x'.\n", optopt);
				return 1;
			default:
				abort();
		}
	
	for(index = optind; index < argc; index++)
		printf("Non-option argument %s\n", argv[index]);

	printf("creating graph\n");
	
	int i, j;
	
	int seed = (int)time(0);
	float random;

	printf("@seed %d\n", seed);	
	srand(seed);

#ifdef	TEST
	// testing case
	n = N;
	int **A;

	A = (int **)calloc(n, sizeof(int *));
	for (i = 0; i < n; i++){
		if ((*(A+i) = (int *)calloc(n, sizeof(int))) == NULL){
			printf("Could not allocate enough memory for matrix A\n");
			exit(3);
		}
	}
	int a[N][N] = {{0, 8, 3, 5, -1},{8, 0 ,2, -1, 5},{-1, 1, 0, 3, 4},{6, -1, -1, 0, 7},{-1, 5, -1, -1, 0}};
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			A[i][j] = a[i][j];
	
#else
	int **A;
	
	A = (int **)calloc(n, sizeof(int *));
	for (i = 0; i < n; i++){
		if ((*(A+i) = (int *)calloc(n, sizeof(int))) == NULL){
			printf("Could not allocate enough memory for matrix A\n");
			exit(3);
		}
	}

	for (i = 0; i < n; i++){
		for (j = 0; j < n; j++)
		{
			random = (float) rand() / (RAND_MAX);
			
			if (i == j)
			{
				A[i][j] = 0;
			}
			else if ((1.0-(float)r/p) < random)
			{
				A[i][j] = (int)(r * random);
			}
			else
			{
				A[i][j] = -1;
			}	
		}
	}
#endif
/*
	for (i = 0; i < n; i++){
		printf("row %d: ", i);
		for (j = 0; j < n; j++){
			if (j == n-1)
				printf("%d\n", A[i][j]);
			else
				printf("%d\t", A[i][j]);
		}
	}
*/
	write_graph(output_file, n, A);

	for (i = 0; i < n; i++){
		free(A[i]);
	}
	free(A);

	return 0;
}
