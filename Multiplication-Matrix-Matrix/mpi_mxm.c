#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#ifdef _OPENMP
#include <omp.h>
#endif


int main(int argc, char **argv)
{	
	int rank, comsz;
	double t0, t1;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comsz);

	int i, j, k;
	int sqrtcomm = sqrt(comsz);
	int shi_c, shi_l;
	int matSize = atoi(argv[1]);
	int l_matSize = matSize / sqrtcomm + (rank < matSize % sqrtcomm ? 1 : 0);

	float *M_1 = (float*) malloc(l_matSize*matSize*sizeof(float));
	if (M_1 == NULL)
	{
		printf("Memory is not allocated in M_1.\n");
		exit(1);
	}
	float *M_2 = (float*) malloc(matSize*l_matSize*sizeof(float));
	if (M_2 == NULL)
	{
		printf("Memory is not allocated in M_2.\n");
		exit(1);
	}
	float *l_M = (float*) malloc(matSize*matSize*sizeof(float));
	if (l_M == NULL)
	{
		printf("Memory is not allocated in l_M.\n");
		exit(1);
	}
	float *M = (float*) malloc(matSize*matSize*sizeof(float));
	if (M == NULL)
	{
		printf("Memory is not allocated in l_M.\n");
		exit(1);
	}

	shi_c = rank%sqrtcomm*l_matSize;
	shi_l = floor(rank/sqrtcomm)*l_matSize;


	MPI_Barrier(MPI_COMM_WORLD);
	t0 = MPI_Wtime();

 	#ifdef _OPENMP
	#pragma omp parallel for collapse(2)
	#endif
	for (i = 0; i < l_matSize; i++)
	{
		for (j = 0; j < l_matSize; j++)
		{
			l_M[(i+shi_c)*matSize + (j+shi_l)] = 0;
			for (k = 0; k < matSize; k++)
			{
				l_M[(i+shi_c)*matSize + (j+shi_l)] += M_1[i*matSize + k] + M_2[k*l_matSize + j];
			}
		}
	}


	MPI_Reduce(M, l_M, matSize*matSize, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);
	t1 = MPI_Wtime();

	if (rank == 0)
		printf("%1.4e\t", t1-t0);

	MPI_Finalize();

	return 0;	
}