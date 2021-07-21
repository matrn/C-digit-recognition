#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"

 #define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define MATRIX_TYPE double

#define MATRIX_WRONG_SIZE -1
#define MATRIX_OK 0

typedef int8_t matrix_rtn;

typedef uint32_t matrix_size_t;

typedef struct Matrix {
	matrix_size_t r;
	matrix_size_t c;
	MATRIX_TYPE* data;
} matrix_t;


void matrix_init(matrix_t *mat);
void matrix_alloc(matrix_t* mat, matrix_size_t rows, matrix_size_t cols);
void matrix_free(matrix_t* mat);
void matrix_resize(matrix_t * mat, matrix_size_t rows, matrix_size_t cols);
void matrix_zero(matrix_t * mat);

matrix_rtn matrix_eye(matrix_t* mat);

void matrix_print(const matrix_t* mat);
void matrix_print_wh(const matrix_t* mat, bool header);  // with header


matrix_rtn matrix_add(matrix_t* out, const matrix_t* a, const matrix_t* b);
matrix_rtn matrix_sub(matrix_t* out, const matrix_t* a, const matrix_t* b);



#endif