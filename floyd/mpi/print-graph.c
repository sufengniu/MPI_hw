#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

int main(int argc, char **argv)
{

	int n;
	int **A;
	int i;
	
	read_graph(argv[1], &n, &A);

	print_graph(n, A);

	for (i = 0; i < n; i++)
	{
		free(A[i]);
	}
	free(A);

	return 0;
}
