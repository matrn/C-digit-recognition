#ifndef MATRIX_H
#define MATRIX_H

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cblas.h>

#include "debug.h"

#define MATRIX_USE_DOUBLE
//#define MATRIX_USE_FLOAT

#define MATRIX_USE_BLAS



#ifdef MATRIX_USE_DOUBLE
	#define MATRIX_TYPE double
#endif
#ifdef MATRIX_USE_FLOAT
	#define MATRIX_TYPE float
	#define cblas_dgemm cblas_sgemm
	#define cblas_dimatcopy cblas_simatcopy
	#define cblas_domatcopy cblas_somatcopy
#endif


#define MATRIX_WRONG_SIZE -1
#define MATRIX_OK 0

typedef int8_t matrix_rtn;

typedef uint32_t matrix_size_t;

typedef struct Matrix {
	matrix_size_t r;
	matrix_size_t c;
	MATRIX_TYPE* data;
} matrix_t;

enum Axis {
	ROW_AXIS = 0,  // 0th axis
	COL_AXIS = 1,  // 1st axis
	X_AXIS = 1,
	Y_AXIS = 0,
};

matrix_t* matrix_new();
void matrix_init(matrix_t* mat);
void matrix_alloc(matrix_t* mat, matrix_size_t rows, matrix_size_t cols);
void matrix_free(matrix_t* mat);
void matrix_delete(matrix_t* mat);
void matrix_copy(matrix_t* dst, const matrix_t* src);
void matrix_resize(matrix_t* mat, matrix_size_t rows, matrix_size_t cols);
void matrix_zero(matrix_t* mat);
matrix_rtn matrix_eye(matrix_t* mat);

matrix_size_t matrix_get_rows(matrix_t* mat);
matrix_size_t matrix_get_cols(matrix_t* mat);

MATRIX_TYPE* matrix_at(const matrix_t* mat, const matrix_size_t row, const matrix_size_t col);
MATRIX_TYPE matrix_atv(const matrix_t* mat, const matrix_size_t row, const matrix_size_t col);
MATRIX_TYPE* matrix_at_index(const matrix_t* mat, const matrix_size_t index);
//uint8_t* matrix_1ubyteMat_at(const uint8_t * mat, const matrix_size_t row, const matrix_size_t col);
//uint8_t matrix_1ubyteMat_atv(const uint8_t * mat, const matrix_size_t row, const matrix_size_t col);

void matrix_print(const matrix_t* mat);
void matrix_print_wh(const matrix_t* mat, bool header);	 // with header
void matrix_print_size(const matrix_t* mat);

matrix_rtn matrix_add(matrix_t* out, const matrix_t* a, const matrix_t* b);
matrix_rtn matrix_sub(matrix_t* out, const matrix_t* a, const matrix_t* b);

matrix_rtn matrix_multiply(matrix_t* out, const matrix_t* a, const matrix_t* b);
matrix_rtn matrix_multiply_ew(matrix_t* out, const matrix_t* a, const matrix_t* b);

MATRIX_TYPE matrix_max(matrix_t* mat);
matrix_size_t matrix_argmax(matrix_t* mat);

MATRIX_TYPE matrix_min(matrix_t* mat);
matrix_size_t matrix_argmin(matrix_t* mat);

matrix_rtn matrix_maximum(matrix_t* out, const matrix_t* mat, const MATRIX_TYPE value);	 // create `out` matrix with max values from `mat` against `value`
matrix_rtn matrix_minimum(matrix_t* out, const matrix_t* mat, const MATRIX_TYPE value);	 // create `out` matrix with min values from `mat` against `value`

matrix_rtn matrix_divide_ew(matrix_t* out, matrix_t* a, matrix_t* b);
matrix_rtn matrix_divide_lscalar(matrix_t* out, MATRIX_TYPE scalar, matrix_t* mat);
matrix_rtn matrix_divide_rscalar(matrix_t* out, matrix_t* mat, MATRIX_TYPE scalar);

matrix_rtn matrix_exp_ew(matrix_t* out, matrix_t* mat);

matrix_rtn matrix_scale(matrix_t* out, const matrix_t* mat, const MATRIX_TYPE scalar);

matrix_rtn matrix_add_scalar(matrix_t* out, const matrix_t* mat, MATRIX_TYPE scalar);
matrix_rtn matrix_sub_lscalar(matrix_t* out, MATRIX_TYPE scalar, matrix_t* mat);
matrix_rtn matrix_sub_rscalar(matrix_t* out, matrix_t* mat, MATRIX_TYPE scalar);

matrix_rtn matrix_random_uniform(matrix_t* out, double min_val, double max_val);

void matrix_transpose(matrix_t* dst, matrix_t* src);

matrix_rtn matrix_multiply_r1ubyteMat(matrix_t* out, matrix_t* a, uint8_t* b, matrix_size_t rows, matrix_size_t cols);
//matrix_rtn matrix_1ubyteMat_crop_edges(uint8_t * out, matrix_size_t * out_rows, matrix_size_t * out_cols, uint8_t * mat, matrix_size_t rows, matrix_size_t cols);
void matrix_1ubyteMat_calculate_crop(int* width_start, int* width_stop, int* height_start, int* height_stop, const uint8_t* mat, const matrix_size_t rows, const matrix_size_t cols, const uint8_t pixel_bytes, const uint8_t null_values[]);
void matrix_1ubyteMat_calculate_crop_old(int* width_start, int* width_stop, int* height_start, int* height_stop, uint8_t* mat, matrix_size_t rows, matrix_size_t cols);
uint8_t* matrix_1ubyteMat_crop_edges(matrix_size_t* out_rows, matrix_size_t* out_cols, uint8_t* mat, matrix_size_t rows, matrix_size_t cols);
uint8_t* matrix_1ubyteMat_add_frame(matrix_size_t* out_rows, matrix_size_t* out_cols, uint8_t* mat, matrix_size_t rows, matrix_size_t cols, matrix_size_t top_rows, matrix_size_t bottom_rows, matrix_size_t left_cols, matrix_size_t right_cols);
void matrix_1ubyteMat_display(const uint8_t* img, const int rows, const int cols);
void matrix_1ubyteMat_print(const uint8_t* img, const int rows, const int cols);

matrix_size_t matrix_1ubyteMat_mean(const uint8_t* mat, const matrix_size_t rows, const matrix_size_t cols, enum Axis axis);
void matrix_1ubyteMat_submat_move(uint8_t* mat, const matrix_size_t rows, const matrix_size_t cols, const matrix_size_t submat_rows, const matrix_size_t submat_cols, matrix_size_t submat_pos_row, matrix_size_t submat_pos_col, int drows, int dcols);

#endif