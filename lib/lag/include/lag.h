#ifndef LAG_H
#define LAG_H

#include "matrix.h"
#include "debug.h"

#define EULER 2.71828182845904523536

/*
#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })
*/
#define max(x, y) (((x) > (y)) ? (x) : (y))
#define min(x, y) (((x) < (y)) ? (x) : (y))


void random_seed(unsigned int seed);


#endif