#include "include/matrix.h"

void matrix_alloc(matrix_t* mat, enum MATRIX_TYPE type, matrix_size_t rows, matrix_size_t cols) {
	mat->type = type;
	mat->r = rows;
	mat->c = cols;

	mat->data = calloc(rows * cols, sizeof(matrix_data_t));
}

void matrix_free(matrix_t* mat) {
	mat->r = mat->c = 0;
	free(mat->data);
	mat->data = NULL;
}

matrix_data_t matrix_at(matrix_t* mat, matrix_size_t row, matrix_size_t col) {
	return mat->data[mat->r * row + col];
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
			case MATRIX_INT:
				printf("int");
				break;
			case MATRIX_UINT:
				printf("unsigned int");
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
				case MATRIX_UINT:
					printf("%8ld", mat->data[mat->r * r + c].u);
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

	memset(mat->data, 0, sizeof(matrix_data_t) * mat->r * mat->c);

	for (int i = 0; i < mat->r; i++) {
		printf("%d\n", mat->type);
		switch (mat->type) {
			case MATRIX_DOUBLE:
				mat->data[mat->r * i + i].d = 1;
				break;
			case MATRIX_INT:
				mat->data[mat->r * i + i].i = 1;
				break;
			case MATRIX_UINT:
				mat->data[mat->r * i + i].u = 1;
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

enum MATRIX_TYPE matrix_choose_type(matrix_t* a, matrix_t* b) {
	if (a->type == MATRIX_DOUBLE || b->type == MATRIX_DOUBLE) return MATRIX_DOUBLE;
	if (a->type == MATRIX_UINT && b->type == MATRIX_UINT) return MATRIX_UINT;
	return MATRIX_INT;
}


matrix_rtn matrix_plus(matrix_t* out, matrix_t* a, matrix_t* b) {
	if (a->r != b->r || a->c != b->c) {
		dbgerrln("Different matrices sizes");
		return MATRIX_WRONG_SIZE;
	}

	out->type = matrix_choose_type(a, b);
	if (out->r != a->r || out->c != a->c) {
		out->r = a->r;
		out->c = a->c;

		if (out->data == NULL) {
			out->data = malloc(sizeof(matrix_data_t) * out->r * out->c);
		} else {
			out->data = relloc(sizeof(matrix_data_t) * out->r * out->c);
		}
	}

	for (int r = 0; r < out->r; r++) {
		for (int c = 0; c < out->c; c++) {

		}
	}

	return MATRIX_OK;
}
