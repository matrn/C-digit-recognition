#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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


matrix_t * matrix_new();

void matrix_init(matrix_t *mat);
void matrix_alloc(matrix_t* mat, matrix_size_t rows, matrix_size_t cols);
void matrix_free(matrix_t* mat);
void matrix_delete(matrix_t * mat);
void matrix_copy(matrix_t * dst, matrix_t * src);
void matrix_resize(matrix_t * mat, matrix_size_t rows, matrix_size_t cols);
void matrix_zero(matrix_t * mat);

matrix_rtn matrix_eye(matrix_t* mat);

void matrix_print(const matrix_t* mat);
void matrix_print_wh(const matrix_t* mat, bool header);  // with header
void matrix_print_size(const matrix_t * mat);


MATRIX_TYPE* matrix_at(const matrix_t* mat, const matrix_size_t row, const matrix_size_t col);
MATRIX_TYPE matrix_atv(const matrix_t* mat, const matrix_size_t row, const matrix_size_t col);	

matrix_rtn matrix_add(matrix_t* out, const matrix_t* a, const matrix_t* b);
matrix_rtn matrix_sub(matrix_t* out, const matrix_t* a, const matrix_t* b);
matrix_rtn matrix_multiply(matrix_t* out, const matrix_t* a, const matrix_t* b);
matrix_rtn matrix_multiply_ew(matrix_t* out, const matrix_t* a, const matrix_t* b);

MATRIX_TYPE matrix_max(matrix_t * mat);
matrix_size_t matrix_argmax(matrix_t * mat);

MATRIX_TYPE matrix_min(matrix_t * mat);
matrix_size_t matrix_argmin(matrix_t * mat);


matrix_rtn matrix_maximum(matrix_t * out, matrix_t * mat, MATRIX_TYPE value);   // create `out` matrix with max values from `mat` against `value`
matrix_rtn matrix_minimum(matrix_t * out, matrix_t * mat, MATRIX_TYPE value);   // create `out` matrix with min values from `mat` against `value`


matrix_rtn matrix_divide_ew(matrix_t * out, matrix_t* a, matrix_t* b);
matrix_rtn matrix_divide_lscalar(matrix_t * out, MATRIX_TYPE scalar, matrix_t * mat);
matrix_rtn matrix_divide_rscalar(matrix_t * out, matrix_t * mat, MATRIX_TYPE scalar);

matrix_rtn matrix_exp_ew(matrix_t * out, matrix_t* mat);


matrix_rtn matrix_scale(matrix_t *out, matrix_t * mat, MATRIX_TYPE scalar);

matrix_rtn matrix_add_scalar(matrix_t * out, const matrix_t * mat, MATRIX_TYPE scalar);
matrix_rtn matrix_sub_lscalar(matrix_t * out, MATRIX_TYPE scalar, matrix_t * mat);
matrix_rtn matrix_sub_rscalar(matrix_t * out, matrix_t * mat, MATRIX_TYPE scalar);

void random_seed(unsigned int seed);

matrix_rtn matrix_random_uniform(matrix_t * out, double min_val, double max_val);


matrix_rtn matrix_multiply_r1ubyteMat(matrix_t * out, matrix_t * a, uint8_t * b, matrix_size_t rows, matrix_size_t cols);


matrix_size_t matrix_get_rows(matrix_t * mat);
matrix_size_t matrix_get_cols(matrix_t * mat);


void matrix_transpose(matrix_t * dst, matrix_t * src);

#endif