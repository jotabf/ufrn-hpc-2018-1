#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#ifndef isAlloc
#define isAlloc(v,c) if (v == NULL)\
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

    int Mx, My, N;				  //size of gridpoints and count time iterations
    int i, j, n;                  //index for x-direction and for time
    int i_l, i_r, j_u, j_d;       //index in space for left, right, up and down side
    double dx, dy, dt;              
    double *c;                    //weve velocity
    double *aux;
    double *u_new, *u_cur, *u_pre;

    #ifdef _OPENMP
    double time_0, time_1;
    #else
    struct timeval now, later;
    #endif

	Mx = atoi(argv[1]);
	My = atoi(argv[2]);
	N = atoi(argv[3]);
	dt = atof(argv[4]);

    dx = 1/Mx;
    dy = 1/My;

    u_new = (double*) malloc((Mx+1)*(My+1)*sizeof(double));
    isAlloc(u_new, "u_new");

    u_cur = (double*) malloc((Mx+1)*(My+1)*sizeof(double));
    isAlloc(u_cur, "u_cur");

    u_pre = (double*) malloc((Mx+1)*(My+1)*sizeof(double));
    isAlloc(u_pre, "u_pre");

    c = (double*) malloc((My+1)*sizeof(double));
    isAlloc(c, "c");

    #ifdef _OPENMP
    time_0 = omp_get_wtime();
    #else      
    gettimeofday(&now, NULL);
    #endif

    #ifdef _OPENMP
    #pragma omp parallel default(none)\
        shared(u_new, u_pre, u_cur, aux, dx, dy, dt, c, Mx, My, N)\
        private(n, i, j, i_l, i_r, j_u, j_d)
    #endif
    {   

        #ifdef _OPENMP
        #pragma omp for schedule(auto) collapse(2)
        #endif
        for (i = 0; i <= Mx; i++)
        {
            for (j = 0; j <= My; j++)
            {
                u_pre[i*My+j] = u_cur[i*My+j] = sin(2*PI*i*dx)*sin(2*PI*j*dy);
            }
        }

        #ifdef _OPENMP
        #pragma omp for schedule(auto)
        #endif
        for (j = 0; j <= My; j++)
        {
            c[j] = 1 + .1 * sin(2*PI*i*dy);
        }

        for (n = 0; n < N; n++)
        {   
            #ifdef _OPENMP
            #pragma omp for schedule(auto) collapse(2)
            #endif
            for (i = 0; i <= Mx; i++)
            {   
                for (j = 0; j <= My; j++)
                {
                    i_l = (i == Mx) ? 1 :i+1;
                    i_r = (i-1 < 0) ? Mx-1:i-1;
                    j_u = (j == My) ? 1 :j+1;
                    j_d = (j-1 < 0) ? My-1:j-1;

                    u_new[i*My+j] = dt*dt*c[j]*c[j]*(u_cur[i_l*My+j] - 2*u_cur[i*My+j] + u_cur[i_r*My+j])/(dx*dx) +
                                    dt*dt*c[j]*c[j]*(u_cur[i*My+j_u] - 2*u_cur[i*My+j] + u_cur[i*My+j_d])/(dy*dy) + 
                                    2 * u_cur[i*My+j] - u_pre[i*My+j];
                }
            }

            #ifdef _OPENMP
            #pragma omp sigle
            #endif
            {
                swap(u_new, u_pre, u_cur);   
            }
        }
    }

    #ifdef _OPENMP
    time_1 = omp_get_wtime();
    #else      
    gettimeofday(&later, NULL);
    #endif

    #ifdef _OPENMP
    printf("%1.4e\t", time_1-time_0);
    #else      
    printf("%1.4e\t", (double)(later.tv_sec - now.tv_sec) + ((double)(later.tv_usec - now.tv_usec))/1.0e6 );
    #endif

	return 0;	
}