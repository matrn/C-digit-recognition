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

#define MATRIX_TYPE_DOUBLE double
#define MATRIX_TYPE_INT int64_t
#define MATRIX_TYPE_UINT uint64_t

#define MATRIX_WRONG_SIZE -1
#define MATRIX_OK 0

enum MATRIX_TYPE { MATRIX_DOUBLE,
				   MATRIX_INT,
				   MATRIX_UINT };

typedef int8_t matrix_rtn;

typedef union {
	MATRIX_TYPE_DOUBLE d;
	MATRIX_TYPE_INT i;
	MATRIX_TYPE_UINT u;
} matrix_data_t;

/*
#define MATRIX_CAST(mat) (\
	##mat->type == MATRIX_DOUBLE ? \
		(MATRIX_TYPE_DOUBLE *)##mat->data : \
	\
	##mat->type == MATRIX_TINYINT ? \
		(MATRIX_TYPE_TINYINT *)##mat->data : \
	\
	##mat->type == MATRIX_INT ? \
		(MATRIX_TYPE_INT *)##mat->data : \
	\
	##mat->type == MATRIX_BIGINT ? \
		(MATRIX_TYPE_BIGINT *)##mat->data : \
	NULL \
)
*/

typedef uint32_t matrix_size_t;

typedef struct Matrix {
	enum MATRIX_TYPE type;
	matrix_size_t r;
	matrix_size_t c;
	matrix_data_t* data;
} matrix_t;

void matrix_alloc(matrix_t* mat, enum MATRIX_TYPE type, matrix_size_t rows, matrix_size_t cols);
void matrix_free(matrix_t* mat);
matrix_rtn matrix_eye(matrix_t* mat);

matrix_data_t matrix_at_double(matrix_t* mat, matrix_size_t row, matrix_size_t col);

void matrix_print(matrix_t* mat);
void matrix_print_wh(matrix_t* mat, bool header);  // with header

enum MATRIX_TYPE matrix_choose_type(matrix_t * a, matrix_t *b);

#endif