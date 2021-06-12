#include "include/matrix.h"

void matrix_alloc(matrix_t* mat, enum MATRIX_TYPE type, matrix_size_t rows, matrix_size_t cols) {
	mat->type = type;
	mat->r = rows;
	mat->c = cols;

	mat->type_size = 0;
	switch (type) {
		case MATRIX_DOUBLE:
			mat->type_size = sizeof(MATRIX_TYPE_DOUBLE);
			break;
		case MATRIX_TINYINT:
			mat->type_size = sizeof(MATRIX_TYPE_TINYINT);
			break;
		case MATRIX_INT:
			mat->type_size = sizeof(MATRIX_TYPE_INT);
			break;
		case MATRIX_BIGINT:
			mat->type_size = sizeof(MATRIX_TYPE_BIGINT);
			break;
	}

	mat->data = calloc(rows * cols, mat->type_size);
}

void matrix_free(matrix_t* mat) {
	mat->r = mat->c = 0;
	free(mat->data);
	mat->data = NULL;
}

double matrix_at_double(matrix_t* mat, matrix_size_t row, matrix_size_t col) {
	switch (mat->type) {
		case MATRIX_DOUBLE:
			return ((MATRIX_TYPE_DOUBLE*)mat->data)[mat->r * row + col];
		case MATRIX_TINYINT:
			return ((MATRIX_TYPE_TINYINT*)mat->data)[mat->r * row + col];
		case MATRIX_INT:
			return ((MATRIX_TYPE_INT*)mat->data)[mat->r * row + col];
		case MATRIX_BIGINT:
			return ((MATRIX_TYPE_BIGINT*)mat->data)[mat->r * row + col];
	}

	return NULL;
}

void matrix_print(matrix_t* mat) {
	matrix_print_wh(mat, false);
}

void matrix_print_wh(matrix_t* mat, bool header) {
	if (header) {
		switch (mat->type) {
			case MATRIX_DOUBLE:
				printf("double");
				break;
			case MATRIX_TINYINT:
				printf("tinyint");
				break;
			case MATRIX_INT:
				printf("int");
				break;
			case MATRIX_BIGINT:
				printf("bigint");
				break;
		}
		printf(": %dx%d\n", mat->r, mat->c);
	}

	for (int r = 0; r < mat->r; r++) {
		for (int c = 0; c < mat->c; c++) {
			switch (mat->type) {
				case MATRIX_DOUBLE:
					printf("%8.3f", ((MATRIX_TYPE_DOUBLE*)mat->data)[mat->r * r + c]);
					break;
				case MATRIX_TINYINT:
					printf("%4d", ((MATRIX_TYPE_TINYINT*)mat->data)[mat->r * r + c]);
					break;
				case MATRIX_INT:
					printf("%6d", ((MATRIX_TYPE_INT*)mat->data)[mat->r * r + c]);
					break;
				case MATRIX_BIGINT:
					printf("%8ld", ((MATRIX_TYPE_BIGINT*)mat->data)[mat->r * r + c]);
					break;
			}
			if (c + 1 < mat->c) printf(" ");
		}
		printf("\n");
	}
}

matrix_rtn matrix_eye(matrix_t* mat) {
	if (mat->r != mat->c) {
		dbgerrln("Matrix must be square");
		return MATRIX_WRONG_SIZE;
	}

	memset(mat->data, 0, mat->type_size*mat->r*mat->c);

	for (int i = 0; i < mat->r; i++) {
		printf("%d\n", mat->type);
		switch (mat->type) {
			case MATRIX_DOUBLE:
				((MATRIX_TYPE_DOUBLE*)mat->data)[mat->r * i + i] = 1;
				break;
			case MATRIX_TINYINT:
				((MATRIX_TYPE_TINYINT*)mat->data)[mat->r * i + i] = 1;
				break;
			case MATRIX_INT:
				((MATRIX_TYPE_INT*)mat->data)[mat->r * i + i] = 1;
				break;
			case MATRIX_BIGINT: 
				((MATRIX_TYPE_BIGINT*)mat->data)[mat->r * i + i] = 1;
				break;
		}
	}

	return MATRIX_OK;
}


matrix_rtn matrix_plus(matrix_t *out, matrix_t *a, matrix_t *b){
	if(a->r != b->r || a->c != b->c){
		dbgerrln("Different matrices sizes");
		return MATRIX_WRONG_SIZE;
	}

	enum MATRIX_TYPE out_type = max(a->type, b->type);

	if(out->data == NULL){
		
	}

	return MATRIX_OK;
}
