#include "include/matrix.h"


matrix_t * matrix_new(){
	return malloc(sizeof(matrix_t));
}


void matrix_init(matrix_t * mat){
	mat->r = mat->c = 0;   // null rows and cols
	mat->data = NULL;
}

void matrix_alloc(matrix_t* mat, matrix_size_t rows, matrix_size_t cols) {
	mat->r = rows;
	mat->c = cols;

	mat->data = (MATRIX_TYPE *)calloc(rows * cols, sizeof(MATRIX_TYPE));
}

void matrix_free(matrix_t* mat) {
	mat->r = mat->c = 0;
	if (mat->data != NULL) free(mat->data);
	mat->data = NULL;
}

void matrix_copy(matrix_t * dst, matrix_t * src){
	matrix_resize(dst, src->r, src->c);
	memcpy(dst->data, src->data, src->r*src->c*sizeof(MATRIX_TYPE));
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
			mat->data = (MATRIX_TYPE *)malloc(sizeof(MATRIX_TYPE) * mat->r * mat->c);
		} else {
			mat->data = (MATRIX_TYPE *)realloc(mat->data, sizeof(MATRIX_TYPE) * mat->r * mat->c);
		}
	}
}

void matrix_zero(matrix_t* mat) {
	memset(mat->data, 0, sizeof(MATRIX_TYPE) * mat->r * mat->c);
}

MATRIX_TYPE* matrix_at(const matrix_t* mat, const matrix_size_t row, const matrix_size_t col) {
	return &mat->data[mat->r * row + col];
}

MATRIX_TYPE matrix_atv(const matrix_t* mat, const matrix_size_t row, const matrix_size_t col) {
	return mat->data[mat->r * row + col];
}


void matrix_print(const matrix_t* mat) {
	matrix_print_wh(mat, false);
}

void matrix_print_wh(const matrix_t* mat, bool header) {
	if (header) {
		printf("Mat: %dx%d\n", mat->r, mat->c);
	}

	for (int r = 0; r < mat->r; r++) {
		for (int c = 0; c < mat->c; c++) {
			printf("%8.2f", mat->data[mat->r * r + c]);
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
		mat->data[mat->r * i + i] = 1;				
	}

	return MATRIX_OK;
}




matrix_rtn matrix_add(matrix_t* out, const matrix_t* a, const matrix_t* b) {
	if (a->r != b->r || a->c != b->c) {
		dbgerrln("Different matrices sizes");
		return MATRIX_WRONG_SIZE;
	}

	matrix_resize(out, a->r, a->c);

	for (int r = 0; r < out->r; r++) {
		for (int c = 0; c < out->c; c++) {
			*matrix_at(out, r, c) = *matrix_at(a, r, c) + *matrix_at(b, r, c);
		}
	}

	return MATRIX_OK;
}

matrix_rtn matrix_sub(matrix_t* out, const matrix_t* a, const matrix_t* b) {
	if (a->r != b->r || a->c != b->c) {
		dbgerrln("Different matrices sizes");
		return MATRIX_WRONG_SIZE;
	}

	matrix_resize(out, a->r, a->c);

	for (int r = 0; r < out->r; r++) {
		for (int c = 0; c < out->c; c++) {
			*matrix_at(out, r, c) = *matrix_at(a, r, c) - *matrix_at(b, r, c);
		}
	}

	return MATRIX_OK;
}



matrix_rtn matrix_multiply(matrix_t* out, const matrix_t* a, const matrix_t* b) {
	if (a->c != b->r) {
		dbgerrln("Wrong matrices sizes");
		return MATRIX_WRONG_SIZE;
	}

	matrix_resize(out, a->r, b->c);

	for (int row = 0; row < a->r; row ++) {
		for (int col = 0; col < a->c; col ++) {
			MATRIX_TYPE sum = 0;
			for(int c = 0; c < b->c; c ++){
				sum += matrix_atv(a, row, c)*matrix_atv(b, c, col);
			}
			printf("%f\n", sum);
			*matrix_at(out, row, col) = sum;
		}
	}

	return MATRIX_OK;
}


matrix_rtn matrix_multiply_ew(matrix_t* out, const matrix_t* a, const matrix_t* b) {
	if (a->r != b->r || a->c != b->c) {
		dbgerrln("Different matrices sizes");
		return MATRIX_WRONG_SIZE;
	}

	matrix_resize(out, a->r, a->c);

	for (int r = 0; r < out->r; r++) {
		for (int c = 0; c < out->c; c++) {
			*matrix_at(out, r, c) = matrix_atv(a, r, c)*matrix_atv(b, r, c);
		}
	}

	return MATRIX_OK;
}


MATRIX_TYPE matrix_max(matrix_t * mat){
	MATRIX_TYPE max_val = mat->data[0];
	for(int i = 1; i < mat->r*mat->c; i ++){
		if(mat->data[i] > max_val) max_val = mat->data[i];
	}
	return max_val;
}

matrix_size_t matrix_argmax(matrix_t * mat){
	MATRIX_TYPE max_val = mat->data[0];
	matrix_size_t max_val_index = 0;
	for(int i = 1; i < mat->r*mat->c; i ++){
		if(mat->data[i] > max_val){
			max_val = mat->data[i];
			max_val_index = i;
		}
	}
	return max_val_index;
}



MATRIX_TYPE matrix_min(matrix_t * mat){
	MATRIX_TYPE min_val = mat->data[0];
	for(int i = 1; i < mat->r*mat->c; i ++){
		if(mat->data[i] < min_val) min_val = mat->data[i];
	}
	return min_val;
}

matrix_size_t matrix_argmin(matrix_t * mat){
	MATRIX_TYPE min_val = mat->data[0];
	matrix_size_t min_val_index = 0;
	for(int i = 1; i < mat->r*mat->c; i ++){
		if(mat->data[i] < min_val){
			min_val = mat->data[i];
			min_val_index = i;
		}
	}
	return min_val_index;
}



matrix_rtn matrix_maximum(matrix_t * out, matrix_t * mat, MATRIX_TYPE value){
	matrix_resize(out, mat->r, mat->c);

	for(int i = 0; i < mat->r*mat->c; i ++){
		out->data[i] = mat->data[i] > value ? mat->data[i] : value;
	}
	return MATRIX_OK;
}


matrix_rtn matrix_minimum(matrix_t * out, matrix_t * mat, MATRIX_TYPE value){
	matrix_resize(out, mat->r, mat->c);

	for(int i = 0; i < mat->r*mat->c; i ++){
		out->data[i] = mat->data[i] < value ? mat->data[i] : value;
	}
	return MATRIX_OK;
}





matrix_rtn matrix_divide_ew(matrix_t * out, matrix_t* a, matrix_t* b){
	if (a->r != b->r || a->c != b->c) {
		dbgerrln("Different matrices sizes");
		return MATRIX_WRONG_SIZE;
	}

	matrix_resize(out, a->r, a->c);

	for(int i = 0; i < a->r*a->c; i ++){
		out->data[i] = a->data[i]*b->data[i];
	}
	return MATRIX_OK;
}

matrix_rtn matrix_divide_lscalar(matrix_t * out, MATRIX_TYPE scalar, matrix_t * mat){
	matrix_resize(out, mat->r, mat->c);

	for(int i = 0; i < mat->r*mat->c; i ++){
		out->data[i] = scalar/mat->data[i];
	}
	return MATRIX_OK;
}


matrix_rtn matrix_divide_rscalar(matrix_t * out, matrix_t * mat, MATRIX_TYPE scalar){
	matrix_resize(out, mat->r, mat->c);

	for(int i = 0; i < mat->r*mat->c; i ++){
		out->data[i] = mat->data[i]/scalar;
	}
	return MATRIX_OK;
}



matrix_rtn matrix_exp_ew(matrix_t * out, matrix_t* mat){
	matrix_resize(out, mat->r, mat->c);

	for(int i = 0; i < mat->r*mat->c; i ++){
		out->data[i] = exp(mat->data[i]);
	}
	return MATRIX_OK;
}


matrix_rtn matrix_scale(matrix_t *out, matrix_t * mat, MATRIX_TYPE scalar){
	matrix_resize(out, mat->r, mat->c);

	for(int i = 0; i < mat->r*mat->c; i ++){
		out->data[i] = scalar*mat->data[i];
	}

	return MATRIX_OK;
}

matrix_rtn matrix_add_scalar(matrix_t * out, const matrix_t * mat, MATRIX_TYPE scalar){
	matrix_resize(out, mat->r, mat->c);

	for(int i = 0; i < mat->r*mat->c; i ++){
		out->data[i] = scalar+mat->data[i];
	}

	return MATRIX_OK;
}

matrix_rtn matrix_sub_lscalar(matrix_t * out, MATRIX_TYPE scalar, matrix_t * mat){
	matrix_resize(out, mat->r, mat->c);

	for(int i = 0; i < mat->r*mat->c; i ++){
		out->data[i] = scalar-mat->data[i];
	}

	return MATRIX_OK;
}

matrix_rtn matrix_sub_rscalar(matrix_t * out, matrix_t * mat, MATRIX_TYPE scalar){
	matrix_resize(out, mat->r, mat->c);

	for(int i = 0; i < mat->r*mat->c; i ++){
		out->data[i] = mat->data[i]-scalar;
	}

	return MATRIX_OK;
}