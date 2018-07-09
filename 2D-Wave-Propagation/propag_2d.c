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
    int i_l, i_r, j_l, j_r;	//index in space for left and right side
    int Mx, My, N;				//size of gridpoints and count time iterations
    double c*;				//weve velocity
    double dx, dy, dt;			//
    double **aux;
    double **u_new, **u_cur, **u_pre;
    
	// FILE *parametres;
	// parametres = fopen("parametres.txt", "r");

	Mx = atoi(argv[1]) + 1;
	My = atoi(argv[2]) + 1;
	N = atoi(argv[3]);
	dx = atof(argv[4]);
	dy = atof(argv[5]);
	dt = atof(argv[6]);
	//c = atof(argv[5]);

	u_new = (double**) malloc(Mx*sizeof(double));
    test_alloc(u_new, "u_new");
    for (j = 0; j < My; j++)
    {
    	u_new[j] = (double*) malloc(My*sizeof(double));
    	test_alloc(u_new[j],"u_new[j]");
    }

    u_cur = (double**) malloc(Mx*sizeof(double));
    test_alloc(u_cur, "u_cur");
    for (j = 0; j < My; j++)
    {
    	u_cur[j] = (double*) malloc(My*sizeof(double));
    	test_alloc(u_cur[j],"u_cur[j]");
    }

    u_pre = (double**) malloc(Mx*sizeof(double));
    test_alloc(u_pre, "u_pre");
    for (j = 0; j < My; j++)
    {
    	u_pre[j] = (double*) malloc(My*sizeof(double));
    	test_alloc(u_pre[j],"u_pre[j]");
    }

    c = (double*) malloc(My*sizeof(double));

    for (i = 0; i < M; i++)
    {
    	//u_cur[i] = sin(2*PI*i*dx);
    	//u_pre[i] = u_cur[i] - dt*2*PI*c*cos(2*PI*i*dx);
    	for (j = 0; j < My; j++)
    	{
    		u_cur[i][j] = sin(2*PI*i*dx)*sin(2*PI*j*dy);
    	}
    }

    for (j = 0; j < My; j++)
    {
    	c[j] = 1 + .1 * sin(2*PI*i*dy);
    }
    
    for (n = 0; n < N; n++)
    {	
    	for (i = 0; i < Mx; i++)
		{	
			i_l =  (i == Mx-1) ? 2 : i+1;
			i_r = (i-1 < 0 ) ? Mx-2:i-1;
			for (j = 0; j < My; j++)
			{
				j_l =  (j == My-1) ? 2 : j+1;
				j_r = (j-1 < 0 ) ? My-2:j-1;
				//u_new[i] = dt*dt*c*c*((u_cur[i_left] - 2*u_cur[i] + u_cur[i_right])/(dx*dx)) + 2*u_cur[i] - u_pre[i];	
			}
		}

		swap(u_new, u_pre, u_cur);	
    } 	
	
	return 0;
	
}