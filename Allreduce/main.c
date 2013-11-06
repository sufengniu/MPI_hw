#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mpi.h"

#define N	10000000000

double global_sum(int rank, int size, double my_value)
{
	
	int i;	// stage index
	
	int mask = 1;
		
	int couple;	// send recv corresponding rank
	double temp;

	for (i = 0 ; i < (int)log2(size); i++)
	{
		// bit mask		
		couple = rank^mask;	
		mask <<= 1;
		
		/*
		MPI_Send(&my_value, 1, MPI_DOUBLE, couple, 0, MPI_COMM_WORLD);
		MPI_Recv(&temp, 1, MPI_DOUBLE, couple, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		*/
		MPI_Sendrecv(	&my_value, 1, MPI_DOUBLE, couple, 0,
				&temp, 1, MPI_DOUBLE, couple, 0,
				MPI_COMM_WORLD, MPI_STATUS_IGNORE
				);
		my_value += temp;
	}
	
	return my_value;

}

int main(int argc, char *argv[])
{
	
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	MPI_Status status;

	// Get the name of the processor
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	printf("processor %s, rank %d out of %d processors\n", processor_name, rank, size);
	fflush(stdout);
	
	/* computation begins */
	// caculate each initial process value
	double gsum, my_value, temp;
	long int i;	
	for (i = 0; i < (long int)(N/size); i++)
	{
		my_value += 1.0;
	}
	
	// debugging
	MPI_Barrier(MPI_COMM_WORLD);
	printf("each process value is %e\n", my_value);
	fflush(stdout);

	double t1, t2;

	t1=MPI_Wtime();	
	gsum = global_sum(rank, size, my_value);	
	t2=MPI_Wtime();

	MPI_Barrier(MPI_COMM_WORLD);
	printf("global sum is %e\n", gsum);
	fflush(stdout);	

	printf("MPI_Wtime measured results: %1.8f seconds\n", t2-t1);fflush(stdout);

	MPI_Finalize();

	return 0;
}
