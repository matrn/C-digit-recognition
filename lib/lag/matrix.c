#include "include/matrix.h"

void matrix_alloc(matrix_t* mat, enum MATRIX_TYPE type, matrix_size_t rows, matrix_size_t cols) {
	mat->type = type;
	mat->r = rows;
	mat->c = cols;

	size_t type_size = 0;
	switch (type) {
		case MATRIX_DOUBLE:
			type_size = sizeof(MATRIX_TYPE_DOUBlE);
			break;
		case MATRIX_TINYINT:
			type_size = sizeof(MATRIX_TYPE_TINYINT);
			break;
		case MATRIX_INT:
			type_size = sizeof(MATRIX_TYPE_INT);
			break;
		case MATRIX_BIGINT:
			type_size = sizeof(MATRIX_TYPE_BIGINT);
			break;
	}

	mat->data = calloc(rows * cols, type_size);
}

void matrix_free(matrix_t* mat) {
	mat->r = mat->c = 0;
	free(mat->data);
}

void* matrix_at(matrix_t* mat, matrix_size_t row, matrix_size_t col){
	switch (mat->type) {
			case MATRIX_DOUBLE:
				reutrn mat->data
				break;
			case MATRIX_TINYINT:
				reutrn mat->data
				break;
			case MATRIX_INT:
				reutrn mat->data
				break;
			case MATRIX_BIGINT:
				reutrn mat->data
				break;
		}
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
			printf("\n");
		}
	}