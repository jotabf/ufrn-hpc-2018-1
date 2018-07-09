#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#define ull unsigned long long

ull * init();
void set_histogram(ull *hist, unsigned char *in, ull size);
void print_h(ull *hist);

#endif