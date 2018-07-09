#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "histogram.h"
//#include "image.h"

#define uchar unsigned char
#define GB (1024 * 1024 * 1024)

int main(int argc, char *argv[])
{
	int rank, comm_sz;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double t0, t1;
	double g_T, l_T;
	ull *l_hist = init();
	ull *g_hist = init();
	uchar *l_image;
	ull g_size = atoi(argv[1]);
	ull l_size;

	t0 = MPI_Wtime();

	l_size = g_size * GB / comm_sz + (rank < g_size * GB % comm_sz ? 1 : 0);
	l_image = (uchar *)malloc(sizeof(uchar) * (l_size));

	set_histogram(l_hist, l_image, l_size);

	MPI_Reduce(l_hist, g_hist, 256, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	t1 = MPI_Wtime();

	l_T = t1 - t0;

	MPI_Reduce(&l_T, &g_T, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

	if (rank == 0)
		printf("%1.3e\t", g_T);
	//if(rank == 0) printf("%llu\t",g_size * GB);
	//if(rank == 0) printf("%llu\n",g_hist[0]);
	//if(rank == 0) print_h(g_hist);

	free(l_hist);
	free(g_hist);
	free(l_image);

	MPI_Finalize();
}