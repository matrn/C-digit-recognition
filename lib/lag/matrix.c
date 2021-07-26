#include "include/matrix.h"




matrix_t * matrix_new(){
	matrix_t * new_mat = malloc(sizeof(matrix_t));
	new_mat->data = NULL;
	new_mat->r = -1;
	new_mat->c = -1;
	return new_mat;
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

void matrix_delete(matrix_t * mat){
	matrix_free(mat);
	free(mat);
}

void matrix_copy(matrix_t * dst, matrix_t * src){
	if(dst == src){
		//dbgln("Destination is same as the source, not copying");
		return;
	}
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
	if(row >= mat->r) dbgerrln("Row is outside of matrix!");
	if(col >= mat->c) dbgerrln("Col is outside of matrix!");
	return &mat->data[row*mat->c + col];
}

MATRIX_TYPE matrix_atv(const matrix_t* mat, const matrix_size_t row, const matrix_size_t col) {
	if(row >= mat->r) dbgerrln("Row is outside of matrix!");
	if(col >= mat->c) dbgerrln("Col is outside of matrix!");
	return mat->data[row*mat->c + col];
}


void matrix_print(const matrix_t* mat) {
	matrix_print_wh(mat, false);
}

void matrix_print_size(const matrix_t * mat){
		printf("Mat: %dx%d\n", mat->r, mat->c);
}

void matrix_print_wh(const matrix_t* mat, bool header) {
	if (header) {
		printf("Mat: %dx%d\n", mat->r, mat->c);
	}

	for (int r = 0; r < mat->r; r++) {
		for (int c = 0; c < mat->c; c++) {
			printf("%8.3f", mat->data[r*mat->c + c]);
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
		for (int col = 0; col < b->c; col ++) {
			MATRIX_TYPE sum = 0;
			for(int r = 0; r < b->r; r ++){
				sum += matrix_atv(a, row, r)*matrix_atv(b, r, col);
			}
			//printf("%f\n", sum);
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
		//printf("%f\n", out->data[i]);
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


void random_seed(unsigned int seed){
	srand(seed);
}


matrix_rtn matrix_random_uniform(matrix_t * out, double min_val, double max_val){
	// note that rand() function is not secure for security purposes

	for(int i = 0; i < out->r*out->c; i ++){
		out->data[i] = rand()/(RAND_MAX/(max_val-min_val)) + min_val;
	}

	return MATRIX_OK;
}





matrix_rtn matrix_multiply_r1ubyteMat(matrix_t * out, matrix_t * a, uint8_t * b, matrix_size_t rows, matrix_size_t cols){
	if (a->c != rows) {
		dbgerrln("Wrong matrices sizes");
		return MATRIX_WRONG_SIZE;
	}

	//matrix_print_wh(out, true);
	//printf("out size: %dx%d\n", matrix_get_rows(out), matrix_get_cols(out));
	cstart(matrix_resize);
	matrix_resize(out, a->r, cols);
	cstop(matrix_resize);
	//printf("out size: %dx%d\n", matrix_get_rows(out), matrix_get_cols(out));
	

	for (int row = 0; row < a->r; row ++) {
		for (int col = 0; col < cols; col ++) {
			MATRIX_TYPE sum = 0;
			for(int r = 0; r < rows; r ++){
				//printf("index: %d\n", c*cols+col);
				//printf("val: %d\n", b[c*cols+col]);
				if(b[r*cols+col] == 0) continue;
				sum += matrix_atv(a, row, r)*b[r*cols+col];
			}
			//printf("%dx%d: %f\n", row, col, sum);
			//printf("val: %f\n", *matrix_at(out, row, col));
			*matrix_at(out, row, col) = sum;
		}
	}

	return MATRIX_OK;
}




matrix_size_t matrix_get_rows(matrix_t * mat){
	return mat->r;
}
matrix_size_t matrix_get_cols(matrix_t * mat){
	return mat->c;
}



/*
void matrix_transpose(matrix_t * dst, matrix_t * src){
	if(src->r == 1 || src->c == 1){
		if(dst == src){
			//dbgln("Dst == src");
		}
		else{
			matrix_copy(dst, src);
		}
		matrix_size_t tmp = dst->r;
		dst->r = dst->c;
		dst->c = tmp;
		return;
	}
	matrix_copy(dst, src);
	matrix_size_t tmp = dst->r;
		dst->r = dst->c;
		dst->c = tmp;

	for (int i = 0; i < dst->r; ++i) {
		for (int j = i; j < dst->c; ++j) {
			MATRIX_TYPE temp = matrix_atv(dst, i, j);
			*matrix_at(dst, i, j) = matrix_atv(dst, j, i);
			*matrix_at(dst, j, i) = temp;
		}
	}
}
*/


void matrix_transpose(matrix_t * dst, matrix_t * src){ //double m[], const unsigned h, const unsigned w){
	if(src->r == 1 || src->c == 1){
		if(dst == src){
			//dbgln("Dst == src");
		}
		else{
			matrix_copy(dst, src);
		}
		matrix_size_t tmp = dst->r;
		dst->r = dst->c;
		dst->c = tmp;
		return;
	}

	matrix_copy(dst, src);
	matrix_size_t tmp = dst->r;
	dst->r = dst->c;
	dst->c = tmp;

	// from: https://softwareengineering.stackexchange.com/a/271722/327700
	for (unsigned start = 0; start <= src->c * src->r - 1; ++start){
		unsigned next = start;
		unsigned i = 0;
		do{
			++i;
			next = (next % src->r) * src->c + next / src->r;
		} while (next > start);

		if (next >= start && i != 1){
			const double tmp = dst->data[start];
			next = start;
			do{
				i = (next % src->r) * src->c + next / src->r;
				dst->data[next] = (i == start) ? tmp : dst->data[i];
				next = i;
			} while (next > start);
		}
	}
}
