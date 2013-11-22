#include <stdio.h>
#include <stdlib.h>

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
