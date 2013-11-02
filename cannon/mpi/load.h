#ifndef LOAD_H_
#define LOAD_H_ 

#include "mpi.h"

int pread_matrix(char *file_name, int *n, double ***A, MPI_Comm comm);

int pfprintf_matrix(char *file_name, int n, double **A, int chunk_size, MPI_Comm comm);

#endif
