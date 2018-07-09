#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>

int main (int argc, char *argv[])
{
  int  rank, comm_sz;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  double time_lim = strtod(argv[1], NULL);
  double t0; 

  double tam;
  double x,y;
  double pi;
  unsigned long long h=0, n=0;
  unsigned long long send[2];
  unsigned long long recv[2] = {0, 0};

  srand(time(NULL)*rank);


  //if (rank == 0) printf("%lf %lf\n", time_lim, time(NULL));

  t0 = time(NULL);

  while(1)
  {
    x = ((double)rand()/RAND_MAX) - 0.5;
    y = ((double)rand()/RAND_MAX) - 0.5;

    tam = sqrt(x*x + y*y);

    if(tam <= 0.5) h++;
    n++;

    if( time(NULL)-t0 > time_lim ) break;
  }

  send[0] = h;
  send[1] = n;

  //printf("Process [%i] : %i pontos\n",rank,n);

  MPI_Reduce( send, recv, 2, MPI_UNSIGNED_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0)
  {
    pi = (4*(double)recv[0])/(double)recv[1];

    printf("%.10lf\t", pi);
  }

  MPI_Finalize();
}