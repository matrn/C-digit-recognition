#include "include/matrix.h"

/* 
  memory representation of the matrix data array:
	example for the 3x2 matrix:
		1 2
		3 4  -> 1 2 3 4 5 6
		5 6
*/

/**
 * @brief allocates new matrix on heap memory and sets default parameters
 * 
 * @return matrix_t* returns pointer to allocated matrix_t memory
 */
matrix_t* matrix_new() {
	matrix_t* new_mat = malloc(sizeof(matrix_t));
	new_mat->data = NULL;
	new_mat->r = -1;
	new_mat->c = -1;
	return new_mat;
}

/**
 * @brief initializes matrix = sets rows & cols to 0 and data to NULL
 * 
 * @param mat pointer to matrix_t matrix
 */
void matrix_init(matrix_t* mat) {
	mat->r = mat->c = 0;  // null rows and cols
	mat->data = NULL;
}

/**
 * @brief allocates data memory of in mat matrix
 * 
 * @param mat pointer to the matrix_t matrix
 * @param[in] rows number of rows to allocate
 * @param[in] cols number of cols to allocate
 */
void matrix_alloc(matrix_t* mat, matrix_size_t rows, matrix_size_t cols) {
	mat->r = rows;
	mat->c = cols;

	mat->data = (MATRIX_TYPE*)calloc(rows * cols, sizeof(MATRIX_TYPE));
}

/**
 * @brief frees data memory of matrix
 * 
 * @param[in] mat pointer to the matrix_t matrix
 */
void matrix_free(matrix_t* mat) {
	mat->r = mat->c = 0;
	if (mat->data != NULL) free(mat->data);
	mat->data = NULL;
}

/**
 * @brief deletes matrix - frees data memory and free whole matrix which is allocated on the heap memory
 * 
 * @param[in] mat pointer to the matrix_t matrix
 */
void matrix_delete(matrix_t* mat) {
	matrix_free(mat);
	free(mat);
}

/**
 * @brief copies memory of src matrix into dst matrix
 * 
 * @param[out] dst pointer to the destination matrix
 * @param[in] src pointer to the source matrix
 */
void matrix_copy(matrix_t* dst, const matrix_t* src) {
	if (dst == src) {
		//dbgln("Destination is same as the source, not copying");
		return;
	}
	matrix_resize(dst, src->r, src->c);
	memcpy(dst->data, src->data, src->r * src->c * sizeof(MATRIX_TYPE));
}

/**
 * @brief resizes matrix to new size rows*cols & also checks sizes to optimize performance
 * 
 * @param mat pointer to the matrix_t matrix
 * @param[in] rows new size - rows
 * @param[in] cols new size - cols
 */
void matrix_resize(matrix_t* mat, matrix_size_t rows, matrix_size_t cols) {
	if (mat->r != rows || mat->c != rows) {
		// check if allocated memory is the same size of required memory (and rows & cols are just swapped)
		if (mat->r * mat->c == rows * cols) {
			mat->r = rows;
			mat->c = cols;
			return;
		}

		mat->r = rows;
		mat->c = cols;

		if (mat->data == NULL) {
			mat->data = (MATRIX_TYPE*)malloc(sizeof(MATRIX_TYPE) * mat->r * mat->c);
		} else {
			mat->data = (MATRIX_TYPE*)realloc(mat->data, sizeof(MATRIX_TYPE) * mat->r * mat->c);
		}
	}
}

/**
 * @brief nulls matrix's data
 * 
 * @param[in] mat 
 */
void matrix_zero(matrix_t* mat) {
	memset(mat->data, 0, sizeof(MATRIX_TYPE) * mat->r * mat->c);
}

/**
 * @brief returns number of rows of the matrix
 * 
 * @param[in] mat 
 * @return matrix_size_t 
 */
matrix_size_t matrix_get_rows(matrix_t* mat) {
	return mat->r;
}

/**
 * @brief returns number of cols of the matrix
 * 
 * @param[in] mat 
 * @return matrix_size_t 
 */
matrix_size_t matrix_get_cols(matrix_t* mat) {
	return mat->c;
}

/**
 * @brief returns pointer to the element at coordinates row x col
 * 
 * @param[in] mat 
 * @param[in] row 
 * @param[in] col 
 * @return MATRIX_TYPE* returns pointer to the element
 */
MATRIX_TYPE* matrix_at(const matrix_t* mat, const matrix_size_t row, const matrix_size_t col) {
	if (row >= mat->r) dbgerrln("Row is outside of matrix!");
	if (col >= mat->c) dbgerrln("Col is outside of matrix!");
	return &mat->data[row * mat->c + col];
}

/**
 * @brief returns value of the element at coordinates row x col
 * 
 * @param[in] mat 
 * @param[in] row 
 * @param[in] col 
 * @return MATRIX_TYPE returns value of the element
 */
MATRIX_TYPE matrix_atv(const matrix_t* mat, const matrix_size_t row, const matrix_size_t col) {
	if (row >= mat->r) dbgerrln("Row is outside of matrix!");
	if (col >= mat->c) dbgerrln("Col is outside of matrix!");
	return mat->data[row * mat->c + col];
}

/**
 * @brief returns pointer to the element at matrix data index
 * 
 * @param[in] mat 
 * @param[in] index 
 * @return MATRIX_TYPE* 
 */
MATRIX_TYPE* matrix_at_index(const matrix_t* mat, const matrix_size_t index) {
	return &mat->data[index];
}

/**
 * @brief prints matrix into stdout
 * 
 * @param[in] mat 
 */
void matrix_print(const matrix_t* mat) {
	matrix_print_wh(mat, false);
}

/**
 * @brief prints size of the matrix
 * 
 * @param[in] mat 
 */
void matrix_print_size(const matrix_t* mat) {
	printf("Mat: %dx%d\n", mat->r, mat->c);
}

/**
 * @brief prints matrix with or without header
 * 
 * @param[in] mat 
 * @param[in] header 
 */
void matrix_print_wh(const matrix_t* mat, bool header) {
	if (header) matrix_print_size(mat);

	for (int r = 0; r < mat->r; r++) {
		for (int c = 0; c < mat->c; c++) {
			printf("%8.3f", mat->data[r * mat->c + c]);
			if (c + 1 < mat->c) printf(" ");
		}
		printf("\n");
	}
}

/**
 * @brief creates identity matrix
 * 
 * @param mat 
 * @return matrix_rtn returns MATRIX_OK if matrix has correct size, MATRIX_WRONG_SIZE otherwise 
 */
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

/**
 * @brief out = a + b
 * 
 * @param[out] out pointer to the output matrix
 * @param[in] a pointer to the first input matrix
 * @param[in] b pointer to the second input matrix
 * @return matrix_rtn returns MATRIX_OK if ok, otherwise MATRIX_WRONG_SIZE
 */
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

/**
 * @brief out = a - b
 * 
 * @param[out] out 
 * @param[in] a 
 * @param[in] b 
 * @return matrix_rtn 
 */
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

/**
 * @brief out = a*b, classical matrix multiplication
 * 
 * @param[out] out 
 * @param[in] a 
 * @param[in] b 
 * @return matrix_rtn 
 */
matrix_rtn matrix_multiply(matrix_t* out, const matrix_t* a, const matrix_t* b) {
	assert(out != a);
	assert(out != b);

	if (a->c != b->r) {
		dbgerrln("Wrong matrices sizes");
		return MATRIX_WRONG_SIZE;
	}

	matrix_resize(out, a->r, b->c);

	#ifdef MATRIX_USE_BLAS
		int M = a->r;
		int N = b->c;
		int K = a->c;
		// see: https://petewarden.files.wordpress.com/2015/04/gemm_corrected.png
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, M, N, K, 1.0, a->data, K, b->data, N, 0.0, out->data, N);	
	#else
		for (int row = 0; row < a->r; row++) {
			for (int col = 0; col < b->c; col++) {
				MATRIX_TYPE sum = 0;
				for (int r = 0; r < b->r; r++) {
					sum += matrix_atv(a, row, r) * matrix_atv(b, r, col);
				}
				*matrix_at(out, row, col) = sum;
			}
		}
	#endif

	return MATRIX_OK;
}

/**
 * @brief element wise matrix multiplication
 * 
 * @param[out] out 
 * @param[in] a 
 * @param[in] b 
 * @return matrix_rtn 
 */
matrix_rtn matrix_multiply_ew(matrix_t* out, const matrix_t* a, const matrix_t* b) {
	if (a->r != b->r || a->c != b->c) {
		dbgerrln("Different matrices sizes");
		return MATRIX_WRONG_SIZE;
	}

	matrix_resize(out, a->r, a->c);

	for (int r = 0; r < out->r; r++) {
		for (int c = 0; c < out->c; c++) {
			*matrix_at(out, r, c) = matrix_atv(a, r, c) * matrix_atv(b, r, c);
		}
	}

	return MATRIX_OK;
}

/**
 * @brief returns maximum value from the matrix
 * 
 * @param[in] mat 
 * @return MATRIX_TYPE 
 */
MATRIX_TYPE matrix_max(matrix_t* mat) {
	MATRIX_TYPE max_val = mat->data[0];
	for (int i = 1; i < mat->r * mat->c; i++) {
		if (mat->data[i] > max_val) max_val = mat->data[i];
	}
	return max_val;
}

/**
 * @brief returns index of the maximum element
 * 
 * @param[in] mat 
 * @return matrix_size_t 
 */
matrix_size_t matrix_argmax(matrix_t* mat) {
	MATRIX_TYPE max_val = mat->data[0];
	matrix_size_t max_val_index = 0;

	for (int i = 1; i < mat->r * mat->c; i++) {
		if (mat->data[i] > max_val) {
			max_val = mat->data[i];
			max_val_index = i;
		}
	}
	return max_val_index;
}

/**
 * @brief returns minimum value from the matrix
 * 
 * @param[in] mat 
 * @return MATRIX_TYPE 
 */
MATRIX_TYPE matrix_min(matrix_t* mat) {
	MATRIX_TYPE min_val = mat->data[0];
	for (int i = 1; i < mat->r * mat->c; i++) {
		if (mat->data[i] < min_val) min_val = mat->data[i];
	}
	return min_val;
}

/**
 * @brief returns index of the minimum element
 * 
 * @param[in] mat 
 * @return matrix_size_t 
 */
matrix_size_t matrix_argmin(matrix_t* mat) {
	MATRIX_TYPE min_val = mat->data[0];
	matrix_size_t min_val_index = 0;

	for (int i = 1; i < mat->r * mat->c; i++) {
		if (mat->data[i] < min_val) {
			min_val = mat->data[i];
			min_val_index = i;
		}
	}
	return min_val_index;
}

/**
 * @brief element wise operation where value of each position = max(value of the element, value)
 * 
 * @param[out] out 
 * @param[in] mat 
 * @param[in] value 
 * @return matrix_rtn 
 */
matrix_rtn matrix_maximum(matrix_t* out, const matrix_t* mat, const MATRIX_TYPE value) {
	matrix_resize(out, mat->r, mat->c);

	for (int i = 0; i < mat->r * mat->c; i++) {
		out->data[i] = mat->data[i] > value ? mat->data[i] : value;
	}
	return MATRIX_OK;
}

/**
 * @brief element wise operation where value of each position = min(value of the element, value)
 * 
 * @param[out] out 
 * @param[in] mat 
 * @param[in] value 
 * @return matrix_rtn 
 */
matrix_rtn matrix_minimum(matrix_t* out, const matrix_t* mat, const MATRIX_TYPE value) {
	matrix_resize(out, mat->r, mat->c);

	for (int i = 0; i < mat->r * mat->c; i++) {
		out->data[i] = mat->data[i] < value ? mat->data[i] : value;
	}
	return MATRIX_OK;
}

/**
 * @brief element wise matrix division
 * 
 * @param[out] out 
 * @param[in] a 
 * @param[in] b 
 * @return matrix_rtn 
 */
matrix_rtn matrix_divide_ew(matrix_t* out, matrix_t* a, matrix_t* b) {
	if (a->r != b->r || a->c != b->c) {
		dbgerrln("Different matrices sizes");
		return MATRIX_WRONG_SIZE;
	}

	matrix_resize(out, a->r, a->c);

	for (int i = 0; i < a->r * a->c; i++) {
		out->data[i] = a->data[i] / b->data[i];
	}
	return MATRIX_OK;
}

/**
 * @brief element wise: out = scalar/mat
 * 
 * @param[out] out 
 * @param[in] scalar 
 * @param[in] mat 
 * @return matrix_rtn 
 */
matrix_rtn matrix_divide_lscalar(matrix_t* out, MATRIX_TYPE scalar, matrix_t* mat) {
	matrix_resize(out, mat->r, mat->c);

	for (int i = 0; i < mat->r * mat->c; i++) {
		out->data[i] = scalar / mat->data[i];
	}
	return MATRIX_OK;
}

/**
 * @brief element wise: out = mat/scalar
 * 
 * @param[out] out 
 * @param[in] mat 
 * @param[in] scalar 
 * @return matrix_rtn 
 */
matrix_rtn matrix_divide_rscalar(matrix_t* out, matrix_t* mat, MATRIX_TYPE scalar) {
	matrix_resize(out, mat->r, mat->c);

	for (int i = 0; i < mat->r * mat->c; i++) {
		out->data[i] = mat->data[i] / scalar;
	}
	return MATRIX_OK;
}

/**
 * @brief element wise exponential of the matrix
 * 
 * @param[out] out 
 * @param[in] mat 
 * @return matrix_rtn 
 */
matrix_rtn matrix_exp_ew(matrix_t* out, matrix_t* mat) {
	matrix_resize(out, mat->r, mat->c);

	for (int i = 0; i < mat->r * mat->c; i++) {
		out->data[i] = exp(mat->data[i]);
	}
	return MATRIX_OK;
}

/**
 * @brief multiplies each element of the matrix by scalar value
 * 
 * @param[out] out 
 * @param[in] mat 
 * @param[in] scalar 
 * @return matrix_rtn 
 */
matrix_rtn matrix_scale(matrix_t* out, const matrix_t* mat, const MATRIX_TYPE scalar) {
	matrix_resize(out, mat->r, mat->c);

	for (int i = 0; i < mat->r * mat->c; i++) {
		out->data[i] = scalar * mat->data[i];
	}

	return MATRIX_OK;
}

/**
 * @brief appends scalar to each element of the matrix
 * 
 * @param[out] out 
 * @param[in] mat 
 * @param[in] scalar 
 * @return matrix_rtn 
 */
matrix_rtn matrix_add_scalar(matrix_t* out, const matrix_t* mat, MATRIX_TYPE scalar) {
	matrix_resize(out, mat->r, mat->c);

	for (int i = 0; i < mat->r * mat->c; i++) {
		out->data[i] = scalar + mat->data[i];
	}

	return MATRIX_OK;
}

/**
 * @brief element wise substract of scalar-mat
 * 
 * @param[out] out 
 * @param[in] scalar 
 * @param[in] mat 
 * @return matrix_rtn 
 */
matrix_rtn matrix_sub_lscalar(matrix_t* out, MATRIX_TYPE scalar, matrix_t* mat) {
	matrix_resize(out, mat->r, mat->c);

	for (int i = 0; i < mat->r * mat->c; i++) {
		out->data[i] = scalar - mat->data[i];
	}

	return MATRIX_OK;
}

/**
 * @brief element wise substract of mat-scalar
 * 
 * @param[out] out 
 * @param[in] mat 
 * @param[in] scalar 
 * @return matrix_rtn 
 */
matrix_rtn matrix_sub_rscalar(matrix_t* out, matrix_t* mat, MATRIX_TYPE scalar) {
	matrix_resize(out, mat->r, mat->c);

	for (int i = 0; i < mat->r * mat->c; i++) {
		out->data[i] = mat->data[i] - scalar;
	}

	return MATRIX_OK;
}

/**
 * @brief generates random floating point value for each element of the out matrix in the range [min_val, max_val]
 * 
 * @param[out] out 
 * @param[in] min_val 
 * @param[in] max_val 
 * @return matrix_rtn 
 * 
 * @warning note that rand() function is not secure for security purposes
 */
matrix_rtn matrix_random_uniform(matrix_t* out, double min_val, double max_val) {
	// note that rand() function is not secure for security purposes

	for (int i = 0; i < out->r * out->c; i++) {
		out->data[i] = rand() / (RAND_MAX / (max_val - min_val)) + min_val;
	}

	return MATRIX_OK;
}

/**
 * @brief matrix multiplication with uint8_t array matrix on the right size
 * 
 * @param[out] out 
 * @param[in] a 
 * @param[in] b 
 * @param[in] rows 
 * @param[in] cols 
 * @return matrix_rtn 
 */
matrix_rtn matrix_multiply_r1ubyteMat(matrix_t* out, matrix_t* a, uint8_t* b, matrix_size_t rows, matrix_size_t cols) {
	assert(out != a);

	if (a->c != rows) {
		dbgerrln("Wrong matrices sizes");
		return MATRIX_WRONG_SIZE;
	}

	matrix_resize(out, a->r, cols);

	#ifdef MATRIX_USE_BLAS
		int M = a->r;
		int N = cols;
		int K = a->c;
		MATRIX_TYPE *b_cp  = malloc(rows*cols*sizeof(MATRIX_TYPE));
		for(int i = 0; i < rows*cols; i ++) b_cp[i] = b[i];   // it's awful but can it be better?
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, M, N, K, 1.0, a->data, K, b_cp, N, 0.0, out->data, N);
		free(b_cp);
	#else	
		for (int row = 0; row < a->r; row++) {
			for (int col = 0; col < cols; col++) {
				MATRIX_TYPE sum = 0;
				for (int r = 0; r < rows; r++) {
					//printf("index: %d\n", c*cols+col);
					//printf("val: %d\n", b[c*cols+col]);
					if (b[r * cols + col] == 0) continue;
					sum += matrix_atv(a, row, r) * b[r * cols + col];
				}
				//printf("%dx%d: %f\n", row, col, sum);
				//printf("val: %f\n", *matrix_at(out, row, col));
				*matrix_at(out, row, col) = sum;
			}
		}
	#endif

	return MATRIX_OK;
}

/**
 * @brief in-place matrix transposition
 * 
 * @param[out] dst 
 * @param[in] src 
 */
void matrix_transpose(matrix_t* dst, matrix_t* src) {  //double m[], const unsigned h, const unsigned w){
	#ifdef MATRIX_USE_BLAS
		if(dst == src){
			// in-place matrix transposition
			cblas_dimatcopy(CblasRowMajor, CblasTrans, src->r, src->c, 1.0, src->data, src->c, src->r);

			//swap dimensions
			matrix_size_t r = src->r;
			src->r = src->c;
			src->c = r;
		}
		else{
			// out-of-place matrix transposition
			matrix_resize(dst, src->c, src->r);
			cblas_domatcopy(CblasRowMajor, CblasTrans, src->r, src->c, 1.0, src->data, src->c, dst->data, src->r);
		}
	#else
		if (src->r == 1 || src->c == 1) {
			if (dst == src) {
				//dbgln("Dst == src");
			} else {
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
		for (unsigned start = 0; start <= src->c * src->r - 1; ++start) {
			unsigned next = start;
			unsigned i = 0;
			do {
				++i;
				next = (next % src->r) * src->c + next / src->r;
			} while (next > start);

			if (next >= start && i != 1) {
				const double tmp = dst->data[start];
				next = start;
				do {
					i = (next % src->r) * src->c + next / src->r;
					dst->data[next] = (i == start) ? tmp : dst->data[i];
					next = i;
				} while (next > start);
			}
		}
	#endif
}

void matrix_1ubyteMat_calculate_crop(int* width_start, int* width_stop, int* height_start, int* height_stop, const uint8_t* mat, const matrix_size_t rows, const matrix_size_t cols, const uint8_t pixel_bytes, const uint8_t null_values[]) {
	matrix_size_t top_rows = 0;
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols * pixel_bytes;) {
			//printf("i: %d\n", col);
			for (int i = 0; i < pixel_bytes; i++) {
				// printf("%d: %d | ", row*cols*pixel_bytes+col, mat[row*cols*pixel_bytes+col] );
				if (mat[row * cols * pixel_bytes + col] != null_values[i]) {
					top_rows = row;
					row = rows;
					col = cols * pixel_bytes;
					break;
				}
				col++;
			}
		}
	}

	matrix_size_t bottom_rows = 0;
	for (int row = rows - 1; row >= 0; row--) {
		for (int col = 0; col < cols * pixel_bytes;) {
			for (int i = 0; i < pixel_bytes; i++) {
				if (mat[row * cols * pixel_bytes + col] != null_values[i]) {
					bottom_rows = row;
					row = -1;
					col = cols * pixel_bytes;
					break;
				}
				col++;
			}
		}
	}

	matrix_size_t left_cols = 0;
	for (int col = 0; col < cols * pixel_bytes;) {
		for (int i = 0; i < pixel_bytes; i++) {
			for (int row = top_rows; row <= bottom_rows; row++) {
				if (mat[row * cols * pixel_bytes + col] != null_values[i]) {
					left_cols = col / pixel_bytes;
					col = cols * pixel_bytes;
					i = pixel_bytes;
					break;
				}
			}
			col++;
		}
	}

	matrix_size_t right_cols = 0;
	for (int col = cols * pixel_bytes - 1; col >= 0;) {
		for (int i = pixel_bytes - 1; i >= 0; i--) {
			for (int row = top_rows; row <= bottom_rows; row++) {
				if (mat[row * cols * pixel_bytes + col] != null_values[i]) {
					right_cols = col / pixel_bytes;
					col = -1;
					i = -1;
					break;
				}
			}
			col--;
		}
	}

	*width_start = left_cols;
	*width_stop = right_cols;

	*height_start = top_rows;
	*height_stop = bottom_rows;
}

void matrix_1ubyteMat_calculate_crop_old(int* width_start, int* width_stop, int* height_start, int* height_stop, uint8_t* mat, matrix_size_t rows, matrix_size_t cols) {
	matrix_size_t top_rows = 0;
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			if (mat[row * cols + col] != 0) {
				top_rows = row;
				row = rows;
				break;
			}
		}
	}

	matrix_size_t bottom_rows = 0;
	for (int row = rows - 1; row >= 0; row--) {
		for (int col = 0; col < cols; col++) {
			if (mat[row * cols + col] != 0) {
				bottom_rows = row;
				row = -1;
				break;
			}
		}
	}

	matrix_size_t left_cols = 0;
	for (int col = 0; col < cols; col++) {
		for (int row = top_rows; row <= bottom_rows; row++) {
			if (mat[row * cols + col] != 0) {
				left_cols = col;
				col = cols;
				break;
			}
		}
	}

	matrix_size_t right_cols = 0;
	for (int col = cols - 1; col >= 0; col--) {
		for (int row = top_rows; row <= bottom_rows; row++) {
			if (mat[row * cols + col] != 0) {
				right_cols = col;
				col = -1;
				break;
			}
		}
	}

	*width_start = left_cols;
	*width_stop = right_cols;

	*height_start = top_rows;
	*height_stop = bottom_rows;
}

uint8_t* matrix_1ubyteMat_crop_edges(matrix_size_t* out_rows, matrix_size_t* out_cols, uint8_t* mat, matrix_size_t rows, matrix_size_t cols) {
	uint8_t* out = NULL;

	matrix_size_t top_rows = 0;
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			if (mat[row * cols + col] != 0) {
				top_rows = row;
				row = rows;
				break;
			}
		}
	}

	matrix_size_t bottom_rows = 0;
	for (int row = rows - 1; row >= 0; row--) {
		for (int col = 0; col < cols; col++) {
			if (mat[row * cols + col] != 0) {
				bottom_rows = row;
				row = -1;
				break;
			}
		}
	}

	matrix_size_t left_cols = 0;
	for (int col = 0; col < cols; col++) {
		for (int row = top_rows; row <= bottom_rows; row++) {
			if (mat[row * cols + col] != 0) {
				left_cols = col;
				col = cols;
				break;
			}
		}
	}

	matrix_size_t right_cols = 0;
	for (int col = cols - 1; col >= 0; col--) {
		for (int row = top_rows; row <= bottom_rows; row++) {
			if (mat[row * cols + col] != 0) {
				right_cols = col;
				col = -1;
				break;
			}
		}
	}
	//printf("top: %d, bottom: %d\n", top_rows, bottom_rows);
	//printf("left: %d, right: %d\n", left_cols, right_cols);

	*out_rows = bottom_rows - top_rows + 1;
	*out_cols = right_cols - left_cols + 1;
	//printf("size: %ld\n", (*out_rows) * (*out_cols) * sizeof(uint8_t));

	out = (uint8_t*)malloc((*out_rows) * (*out_cols) * sizeof(uint8_t));
	for (int row = top_rows; row <= bottom_rows; row++) {
		for (int col = left_cols; col <= right_cols; col++) {
			//printf("i: %d\n", (row - top_rows) * (*out_cols) + (col - left_cols));
			out[(row - top_rows) * (*out_cols) + (col - left_cols)] = mat[row * cols + col];
		}
	}

	return out;
}

/**
 * @brief add frame around input uint8_t matrix
 * 
 * @param[out] out_rows pointer where output number of output rows will be stored
 * @param[out] out_cols pointer where output number of output cols will be stored
 * @param[in] mat input uint8_t array matrix
 * @param[in] rows number of rows of the mat
 * @param[in] cols number of cols of the mat
 * @param[in] top_rows frame top rows
 * @param[in] bottom_rows frame bottom rows
 * @param[in] left_cols frame left columns
 * @param[in] right_cols frame right columns
 * @return uint8_t* retunrs newly allocated uint8_t matrix which has to be freed using free() function
 */
uint8_t* matrix_1ubyteMat_add_frame(matrix_size_t* out_rows, matrix_size_t* out_cols, uint8_t* mat, matrix_size_t rows, matrix_size_t cols, matrix_size_t top_rows, matrix_size_t bottom_rows, matrix_size_t left_cols, matrix_size_t right_cols) {
	*out_rows = rows + top_rows + bottom_rows;
	*out_cols = cols + left_cols + right_cols;
	uint8_t* out = (uint8_t*)calloc((*out_rows) * (*out_cols), sizeof(uint8_t));

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			out[(row + top_rows) * (*out_cols) + left_cols + col] = mat[row * cols + col];
		}
	}

	return out;
}

/**
 * @brief display uint8_t grayscale matrix using ASCII art
 * 
 * @param[in] img pointer to the data array
 * @param[in] rows
 * @param[in] cols 
 */
void matrix_1ubyteMat_display(const uint8_t* img, const int rows, const int cols) {
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			if (img[row * cols + col] == 0)
				printf(" ");
			else
				printf("█");
		}
		printf("\n");
	}
}

/**
 * @brief print uint8_t matrix
 * 
 * @param[in] img pointer to the data array
 * @param[in] rows
 * @param[in] cols 
 */
void matrix_1ubyteMat_print(const uint8_t* img, const int rows, const int cols) {
	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < cols; col++) {
			printf("%4d ", img[row * cols + col]);
		}
		printf("\n");
	}
}

/**
 * @brief calculates mean value across specified axis. Used for calculation center of the pixel mass
 * 
 * @param mat pointer to the data array
 * @param rows 
 * @param cols 
 * @param axis axis enum: ROW_AXIS/COL_AXIS or Y_AXIS/X_AXIS
 * @return matrix_size_t returns rounded coordinate
 */
matrix_size_t matrix_1ubyteMat_mean(const uint8_t* mat, const matrix_size_t rows, const matrix_size_t cols, enum Axis axis) {
	// see: https://www.researchgate.net/figure/Example-of-calculation-of-the-centre-of-mass-of-an-image-a-The-original-square-image_fig2_233397383

	int m_sum = 0;
	int mr_sum = 0;
	switch (axis) {
		case ROW_AXIS:
			for (int row = 0; row < rows; row++) {
				int m = 0;
				for (int col = 0; col < cols; col++) {
					m += mat[row * cols + col];
				}
				m_sum += m;
				mr_sum += m * (row + 1);
			}
			break;

		case COL_AXIS:
			for (int col = 0; col < cols; col++) {
				int m = 0;
				for (int row = 0; row < rows; row++) {
					m += mat[row * cols + col];
				}
				m_sum += m;
				mr_sum += m * (col + 1);
			}
			break;
	}

	//printf("m_sum: %d\n", m_sum);
	//printf("mr_sum: %d\n", mr_sum);
	return round((double)mr_sum / m_sum);
}

/**
 * @brief move sub-matrix in the matrix
 * 
 * @param mat 
 * @param rows 
 * @param cols 
 * @param submat_rows number of rows of the sub matrix
 * @param submat_cols number of cols of the sub matrix
 * @param submat_pos_row [0,0] position of the submatrix in the matrix
 * @param submat_pos_col [0,0] position of the submatrix in the matrix
 * @param drows number of rows to move (+ up, - down)
 * @param dcols number of cols to move (+ roght, - down)
 */
void matrix_1ubyteMat_submat_move(uint8_t* mat, const matrix_size_t rows, const matrix_size_t cols, const matrix_size_t submat_rows, const matrix_size_t submat_cols, matrix_size_t submat_pos_row, matrix_size_t submat_pos_col, int drows, int dcols) {
	/* rows */
	if (drows > 0) {
		dbgln("row up");
		// move up
		if (drows > submat_pos_row) drows = submat_pos_row;
		//memmove(&mat[(submat_pos_row-drows)*cols], &mat[(submat_pos_row)*cols], drows*cols*sizeof(uint8_t));
		//memset(&mat[(submat_pos_row+submat_rows-drows)*cols], 0, drows*cols*sizeof(uint8_t));
		for (int r = 0; r < submat_cols; r++) {
			memcpy(&mat[(r + submat_pos_row - drows) * cols + submat_pos_col], &mat[(r + submat_pos_row) * cols + submat_pos_col], submat_cols * sizeof(uint8_t));
		}
		for (int r = 0; r < drows; r++) {
			memset(&mat[(r + submat_pos_row + submat_rows - drows) * cols + submat_pos_col], 0, submat_cols * sizeof(uint8_t));
		}

		submat_pos_row -= drows;
	}

	else if (drows < 0) {
		dbgln("row down");
		// move down
		int drows_abs = abs(drows);
		if (drows_abs > rows - submat_pos_row - submat_rows) drows_abs = rows - submat_pos_row - submat_rows;
		//memmove(&mat[(submat_pos_row+drows)*cols], &mat[(submat_pos_row)*cols], submat_rows*cols*sizeof(uint8_t));
		//memset(&mat[(submat_pos_row)*cols], 0, drows_abs*cols*sizeof(uint8_t));

		for (int r = submat_rows - 1; r >= 0; r--) {
			memcpy(&mat[(r + submat_pos_row + drows_abs) * cols + submat_pos_col], &mat[(r + submat_pos_row) * cols + submat_pos_col], submat_cols * sizeof(uint8_t));
		}
		for (int r = 0; r < drows_abs; r++) {
			memset(&mat[(r + submat_pos_row) * cols + submat_pos_col], 0, submat_cols * sizeof(uint8_t));
		}

		submat_pos_row += drows_abs;
	}

	/* cols */
	if (dcols > 0) {
		dbgln("col right");
		// move right
		if (dcols > cols - submat_pos_col - submat_cols) dcols = cols - submat_pos_col - submat_cols;

		for (int r = 0; r < submat_rows; r++) {
			memmove(&mat[(submat_pos_row + r) * cols + submat_pos_col + dcols], &mat[(submat_pos_row + r) * cols + submat_pos_col], submat_cols * sizeof(uint8_t));
			memset(&mat[(submat_pos_row + r) * cols + submat_pos_col], 0, dcols * sizeof(uint8_t));
		}
	} else if (dcols < 0) {
		dbgln("col left");
		// move left
		int dcols_abs = abs(dcols);
		if (dcols_abs > submat_pos_col) dcols_abs = submat_pos_col;

		for (int r = 0; r < submat_rows; r++) {
			memmove(&mat[(submat_pos_row + r) * cols + submat_pos_col - dcols_abs], &mat[(submat_pos_row + r) * cols + submat_pos_col], submat_cols * sizeof(uint8_t));
			memset(&mat[(submat_pos_row + r) * cols + submat_pos_col + submat_cols - dcols_abs], 0, dcols_abs * sizeof(uint8_t));
		}
	}
}