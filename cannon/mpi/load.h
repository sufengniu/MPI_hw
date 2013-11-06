#ifndef LOAD_H_
#define LOAD_H_ 

#include "mpi.h"

int pread_matrix(char *file_name, int *n, int *srow, int *scol, int msize, double ***A, double **Astorage, MPI_Comm comm);

int fprintf_matrix(char *file_name, int n, double *Cstorage);

#endif
