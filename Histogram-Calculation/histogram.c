#include "histogram.h"

#include <stdlib.h>
#include <stdio.h>

ull* init()
{
    ull* hist = (ull *)malloc(sizeof(ull) * 256);
    for (int i = 0; i < 256; i++) hist[i] = 0;

    return hist;
}

void set_histogram(ull *hist, unsigned char *in, ull size)
{
    ull i;
    
    #ifdef _OPENMP
    #pragma omp parallel for default(none)\
     reduction(+:hist[0:255]) shared(in, size) private(i)
    #endif
    for (i = 0; i < size; i++)
        hist[(int)in[i]%256]++;
}

void print_h(ull *hist)
{
    printf("\n");
    for(int i=0; i<256; i++)
    {
        printf(" [%i]: ",i);
        /*for(int j=0; j<hist[i]; j+=150)
        {
            printf("|");
        }*/
        printf("%i",hist[i]);
        printf("\n");
    }
}