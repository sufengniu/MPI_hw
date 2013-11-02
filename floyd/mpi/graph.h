#ifndef GRAPH_H_
#define GRAPH_H_ 

#define PARAL_MODE

#ifdef PARAL_MODE
#include "mpi.h"
#endif

int read_graph(char *file_name, int *n, int ***A);

#ifdef PARAL_MODE
int pread_graph(char *file_name, int *n, int ***A, MPI_Comm comm);
#endif

int write_graph(char *file_name, int n, int **A);

#ifdef PARAL_MODE
int pwrite_graph(char *file_name, int n, int **A, int chunk_size, MPI_Comm comm);
#endif

int print_graph(int n, int **A);


#endif
