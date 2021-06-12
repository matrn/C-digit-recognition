#include "include/matrix.h"

void matrix_init(matrix_t* mat) {
	mat->r = mat->c = 0;
	mat->type = -1;
	mat->data = NULL;
}

void matrix_alloc(matrix_t* mat, enum MATRIX_TYPE type, matrix_size_t rows, matrix_size_t cols) {
	mat->type = type;
	mat->r = rows;
	mat->c = cols;

	mat->data = calloc(rows * cols, sizeof(matrix_data_t));
}

void matrix_free(matrix_t* mat) {
	mat->r = mat->c = 0;
	if (mat->data != NULL) free(mat->data);
	mat->data = NULL;
}

void matrix_resize(matrix_t* mat, matrix_size_t rows, matrix_size_t cols) {
	if (mat->r != rows || mat->c != rows) {
		if (mat->r * mat->c == rows * cols) {
			mat->r = rows;
			mat->c = cols;
			return;
		}

		mat->r = rows;
		mat->c = cols;

		if (mat->data == NULL) {
			mat->data = malloc(sizeof(matrix_data_t) * mat->r * mat->c);
		} else {
			mat->data = realloc(mat->data, sizeof(matrix_data_t) * mat->r * mat->c);
		}
	}
}

void matrix_zero(matrix_t* mat) {
	memset(mat->data, 0, sizeof(matrix_data_t) * mat->r * mat->c);
}

matrix_data_t* matrix_at(matrix_t* mat, matrix_size_t row, matrix_size_t col) {
	return &mat->data[mat->r * row + col];
}

MATRIX_TYPE_DOUBLE matrix_at_double(const matrix_t* mat, matrix_size_t row, matrix_size_t col) {
	if (mat->type == MATRIX_DOUBLE) return mat->data[mat->r * row + col].d;
	return mat->data[mat->r * row + col].i;
}

MATRIX_TYPE_INT matrix_at_int(const matrix_t* mat, matrix_size_t row, matrix_size_t col) {
	if (mat->type == MATRIX_DOUBLE) return mat->data[mat->r * row + col].d;
	return mat->data[mat->r * row + col].i;
}

void matrix_print(const matrix_t* mat) {
	matrix_print_wh(mat, false);
}

void matrix_print_wh(const matrix_t* mat, bool header) {
	if (header) {
		switch (mat->type) {
			case MATRIX_DOUBLE:
				printf("double");
				break;
			case MATRIX_INT:
				printf("int");
				break;
		}
		printf(": %dx%d\n", mat->r, mat->c);
	}

	for (int r = 0; r < mat->r; r++) {
		for (int c = 0; c < mat->c; c++) {
			switch (mat->type) {
				case MATRIX_DOUBLE:
					printf("%8.3f", mat->data[mat->r * r + c].d);
					break;
				case MATRIX_INT:
					printf("%8ld", mat->data[mat->r * r + c].i);
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

	matrix_zero(mat);

	for (int i = 0; i < mat->r; i++) {
		printf("%d\n", mat->type);
		switch (mat->type) {
			case MATRIX_DOUBLE:
				mat->data[mat->r * i + i].d = 1;
				break;
			case MATRIX_INT:
				mat->data[mat->r * i + i].i = 1;
				break;
		}
	}

	return MATRIX_OK;
}

// double + anything = double
// int + int/uint = int
// uint + uint = uint

// double - anything = double
// int/uint - int/uint = int

// double * anything

enum MATRIX_TYPE matrix_choose_type(const matrix_t* a, const matrix_t* b) {
	if (a->type == MATRIX_DOUBLE || b->type == MATRIX_DOUBLE) return MATRIX_DOUBLE;
	//if (a->type == MATRIX_UINT && b->type == MATRIX_UINT) return MATRIX_UINT;
	return MATRIX_INT;
}

matrix_rtn matrix_add(matrix_t* out, const matrix_t* a, const matrix_t* b) {
	if (a->r != b->r || a->c != b->c) {
		dbgerrln("Different matrices sizes");
		return MATRIX_WRONG_SIZE;
	}

	out->type = matrix_choose_type(a, b);
	matrix_resize(out, a->r, a->c);

	for (int r = 0; r < out->r; r++) {
		for (int c = 0; c < out->c; c++) {
			if (out->type == MATRIX_DOUBLE) {
				matrix_at(out, r, c)->d = matrix_at_double(a, r, c) + matrix_at_double(b, r, c);
			} else {
				matrix_at(out, r, c)->i = matrix_at_int(a, r, c) + matrix_at_int(b, r, c);
			}
		}
	}

	return MATRIX_OK;
}
