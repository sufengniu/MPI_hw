#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "graph.h"

// #define TEST

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#define	BILLION	1000000000L;

int main(int argc, char **argv)
{

	int n;
	int **A, **B;
	int i, j, k;
	struct timespec start, stop;
	double accum;

	read_graph(argv[1], &n, &A);	

	B = (int **)calloc(n, sizeof(int *));
	for (i = 0; i < n; i++){
		if ((*(B+i) = (int *)calloc(n, sizeof(int))) == NULL){
                        printf("Could not allocate enough memory for matrix B\n");
			exit(3);
		}
	}

	for (i = 0; i < n; i++){
		for (j = 0; j < n; j ++){
			B[i][j] = A[i][j];
		}
	}

	int dist = 0;

	printf("start computation\n");	

	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start);	
	for (k = 0; k < n; k++){
		for (i = 0; i < n; i++){
			for (j = 0; j < n; j++){
		/*		
				if ((B[i][k] == -1) || (B[k][j] == -1))
					dist = -1;
				else
					dist = B[i][k] + B[k][j];

				if ((dist == -1) || (B[i][j] == -1))
					B[i][j] = -dist * B[i][j];
				else if ((dist == -1) && (B[i][j] == -1))
					B[i][j] = -1;
				else
					B[i][j] = min(B[i][j], dist);
		 */

				if ((B[i][k] == -1) || (B[k][j] == -1))
					B[i][j] = B[i][j];
				else if (B[i][j] == -1)
					B[i][j] = B[i][k] + B[k][j];
				else
					B[i][j] = min(B[i][j], B[i][k] + B[k][j]);
			}
		}
	}
	clock_gettime(CLOCK_THREAD_CPUTIME_ID, &stop);
	accum = (stop.tv_sec - start.tv_sec)+(double)(stop.tv_nsec-start.tv_nsec)/(double)BILLION;
	printf("computation done in %lf second\n", accum);

#ifdef TEST
	printf("\n-- original matrix:\n");
	print_graph(n, A);
	printf("\n-- results:\n");
	print_graph(n, B);
#endif

	write_graph(argv[2], n, B);

	for (i = 0; i < n; i++)
	{
		free(B[i]);
	}
	free(B);	

	for (i = 0; i < n; i++)
	{
		free(A[i]);
	}
	free(A);

	return 0;
}
