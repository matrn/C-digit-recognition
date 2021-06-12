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

#define MATRIX_WRONG_SIZE -1
#define MATRIX_OK 0

enum MATRIX_TYPE { MATRIX_DOUBLE,
				   MATRIX_INT};

typedef int8_t matrix_rtn;

typedef union {
	MATRIX_TYPE_DOUBLE d;
	MATRIX_TYPE_INT i;
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

#define MATRIX_AT(mat, row, col) (\
	mat->type == MATRIX_DOUBLE ? \
		matrix_at(mat, row, col)->d : \
		matrix_at(mat, row, col)->i \
)

typedef uint32_t matrix_size_t;

typedef struct Matrix {
	enum MATRIX_TYPE type;
	matrix_size_t r;
	matrix_size_t c;
	matrix_data_t* data;
} matrix_t;


void matrix_init(matrix_t *mat);
void matrix_alloc(matrix_t* mat, enum MATRIX_TYPE type, matrix_size_t rows, matrix_size_t cols);
void matrix_free(matrix_t* mat);
void matrix_resize(matrix_t * mat, matrix_size_t rows, matrix_size_t cols);
void matrix_zero(matrix_t * mat);

matrix_rtn matrix_eye(matrix_t* mat);

/*
enum MATRIX_TYPE matrix_choose_type(matrix_t * a, matrix_t *b);
matrix_data_t* matrix_at(matrix_t* mat, matrix_size_t row, matrix_size_t col);
MATRIX_TYPE_DOUBLE matrix_at_double(matrix_t* mat, matrix_size_t row, matrix_size_t col);
MATRIX_TYPE_INT matrix_at_int(matrix_t* mat, matrix_size_t row, matrix_size_t col);
*/

void matrix_print(const matrix_t* mat);
void matrix_print_wh(const matrix_t* mat, bool header);  // with header


matrix_rtn matrix_add(matrix_t* out, const matrix_t* a, const matrix_t* b);
matrix_rtn matrix_sub(matrix_t* out, const matrix_t* a, const matrix_t* b);



#endif