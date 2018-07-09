#include <stdlib.h>
#include <stdio.h>
//#include <mpi.h>
#include <math.h>
#include <time.h>

#ifndef test_alloc
#define test_alloc(v,c) if (v == NULL)\
	{\
		printf("Memory is not allocated in %s.\n",c);\
		exit(1);\
	}
#endif

#ifndef swap
#define swap(x,y,z) aux = x;\
					x = y;\
					y = z;\
					z = aux;
#endif

#ifndef PI
#define PI 3.14159265359  
#endif

int main(int argc, char const *argv[])
{	
	// int rank, comsz;
	
	// MPI_Init(&argc, &argv);
	// MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 	// MPI_Comm_size(MPI_COMM_WORLD, &comsz);

    int i, n; 				//index for x-direction and for time
    int i_left, i_right;	//index in space for left and right side
    int M, N;				//size of gridpoints and count time iterations
    double c;				//weve velocity
    double dx, dt;			//
    double *aux;
    double *u_new, *u_cur, *u_pre;
    
	// FILE *parametres;
	// parametres = fopen("parametres.txt", "r");

	M = atoi(argv[1]) + 1;
	N = atoi(argv[2]);
	dx = atof(argv[3]);
	dt = atof(argv[4]);
	c = atof(argv[5]);

	u_new = (double*) malloc(M*sizeof(double));
    test_alloc(u_new, "u_new");

    u_cur = (double*) malloc(M*sizeof(double));
    test_alloc(u_cur, "u_cur");

    u_pre = (double*) malloc(M*sizeof(double));
    test_alloc(u_pre, "u_pre");


    for (i = 0; i < M; i++)
    {
    	u_cur[i] = sin(2*PI*i*dx);
    	u_pre[i] = u_cur[i] - dt*2*PI*c*cos(2*PI*i*dx);
    }
	for(i = 0; i < M; i++)
	{
		printf("%lf\t", u_pre[i]);
	}
	printf("\n");

	for(i = 0; i < M; i++)
	{
		printf("%lf\t", u_cur[i]);
	}
	printf("\n");
    
    for (n = 0; n < N; n++)
    {	
    	for (i = 0; i < M; i++)
		{	
			i_left =  (i == M-1) ? 2 : i+1;
			i_right = (i-1 < 0 ) ? M-2:i-1;
			u_new[i] = dt*dt*c*c*((u_cur[i_left] - 2*u_cur[i] + u_cur[i_right])/(dx*dx)) + 2*u_cur[i] - u_pre[i];
		}

		swap(u_new, u_pre, u_cur);	

		for(i = 0; i < M; i++)
		{
			printf("%lf\t", u_cur[i] - (sin(2*PI*i*dx)*cos(2*PI*c*i*dt)));
		}
		printf("\n");
    } 	
	
	return 0;
	
}