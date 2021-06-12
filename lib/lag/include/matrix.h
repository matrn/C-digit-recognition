#ifndef MATRIX_H
#define MATRIX_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MATRIX_TYPE_DOUBlE double
#define MATRIX_TYPE_TINYINT int8_t
#define MATRIX_TYPE_INT int32_t
#define MATRIX_TYPE_BIGINT int64_t

enum MATRIX_TYPE { MATRIX_DOUBLE,
				   MATRIX_BIGINT,
				   MATRIX_INT,
				   MATRIX_TINYINT };

typedef uint32_t matrix_size_t;

typedef struct Matrix {
	enum MATRIX_TYPE type;
	matrix_size_t r;
	matrix_size_t c;
	void* data;
} matrix_t;

void matrix_alloc(matrix_t* mat, enum MATRIX_TYPE type, matrix_size_t rows, matrix_size_t cols);
void matrix_free(matrix_t* mat);
void* matrix_at(matrix_t* mat, matrix_size_t row, matrix_size_t col);
void matrix_print(matrix_t* mat);
void matrix_print_wh(matrix_t* mat, bool header);  // with header

#endif