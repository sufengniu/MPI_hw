#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

int main(int argc, char *argv[])
{

	int size, rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Status status;
	MPI_File infile;
	MPI_Offset filesize;

	/* parallel read test */
	MPI_File_open(MPI_COMM_WORLD, "matrixD.dat", MPI_MODE_CREATE | MPI_MODE_RDONLY, MPI_INFO_NULL, &infile);
	MPI_File_get_size(infile, &filesize,);	/* in bytes */
		
	filesize = (filesize / sizeof(int)) - 1;
	bufsize = filesize / size + 1;	
	buf = (int *)malloc(bufsize * sizeof(int));

	MPI_File_set_view(infile, ranki*bufsize*sizeof(int), MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
	MPI_File_read(infile, buf, bufsize, MPI_INT, &status);

	MPI_Get_count(&status, MPI_INT, &count);

	

	MPI_File_close(&infile);

	MPI_Finalize();

	return 0;
}
