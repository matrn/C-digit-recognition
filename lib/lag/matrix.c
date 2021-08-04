#include "include/matrix.h"



/**
 * @brief allocates new matrix on heap memory and sets default parameters
 * 
 * @return matrix_t* returns pointer to allocated matrix_t memory
 */
matrix_t * matrix_new(){
	matrix_t * new_mat = malloc(sizeof(matrix_t));
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
void matrix_init(matrix_t * mat){
	mat->r = mat->c = 0;   // null rows and cols
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

	mat->data = (MATRIX_TYPE *)calloc(rows * cols, sizeof(MATRIX_TYPE));
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
void matrix_delete(matrix_t * mat){
	matrix_free(mat);
	free(mat);
}

/**
 * @brief copies memory of src matrix into dst matrix
 * 
 * @param[out] dst pointer to the destination matrix
 * @param[in] src pointer to the source matrix
 */
void matrix_copy(matrix_t * dst, const matrix_t * src){
	if(dst == src){
		//dbgln("Destination is same as the source, not copying");
		return;
	}
	matrix_resize(dst, src->r, src->c);
	memcpy(dst->data, src->data, src->r*src->c*sizeof(MATRIX_TYPE));
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
			mat->data = (MATRIX_TYPE *)malloc(sizeof(MATRIX_TYPE) * mat->r * mat->c);
		} else {
			mat->data = (MATRIX_TYPE *)realloc(mat->data, sizeof(MATRIX_TYPE) * mat->r * mat->c);
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
 * @brief returns pointer to the element at coordinates row x col
 * 
 * @param[in] mat 
 * @param[in] row 
 * @param[in] col 
 * @return MATRIX_TYPE* returns pointer to the element
 */
MATRIX_TYPE* matrix_at(const matrix_t* mat, const matrix_size_t row, const matrix_size_t col) {
	if(row >= mat->r) dbgerrln("Row is outside of matrix!");
	if(col >= mat->c) dbgerrln("Col is outside of matrix!");
	return &mat->data[row*mat->c + col];
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



matrix_rtn matrix_maximum(matrix_t * out, const matrix_t * mat, const MATRIX_TYPE value){
	matrix_resize(out, mat->r, mat->c);

	for(int i = 0; i < mat->r*mat->c; i ++){
		out->data[i] = mat->data[i] > value ? mat->data[i] : value;
	}
	return MATRIX_OK;
}


matrix_rtn matrix_minimum(matrix_t * out, const matrix_t * mat, const MATRIX_TYPE value){
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


matrix_rtn matrix_scale(matrix_t *out, const matrix_t * mat, const MATRIX_TYPE scalar){
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


void matrix_1ubyteMat_calculate_crop(int * width_start, int * width_stop, int * height_start, int * height_stop, uint8_t * mat, matrix_size_t rows, matrix_size_t cols, uint8_t pixel_bytes, const uint8_t null_values[]){
	//matrix_size_t index;
	
	matrix_size_t top_rows = 0;
	//index = 0;
	puts("A");
	for(int row = 0; row < rows; row ++){
		for(int col = 0; col < cols*pixel_bytes;){
			//printf("i: %d\n", col);
			for(int i = 0; i < pixel_bytes; i ++){
				// printf("%d: %d | ", row*cols*pixel_bytes+col, mat[row*cols*pixel_bytes+col] );
				if(mat[row*cols*pixel_bytes+col] != null_values[i]){
					top_rows = row;
					row = rows;
					col = cols*pixel_bytes;
					break;
				}
				col ++;
			}
		}
	}

	matrix_size_t bottom_rows = 0;
	//index = 0;
	puts("B");
	for(int row = rows-1; row >= 0; row --){
		for(int col = 0; col < cols*pixel_bytes;){
			for(int i = 0; i < pixel_bytes; i ++){
				if(mat[row*cols*pixel_bytes+col] != null_values[i]){
					bottom_rows = row;
					row = -1;
					col = cols*pixel_bytes;
					break;
				}
				col ++;
			}
		
		}
	}

	matrix_size_t left_cols = 0;
	//index = 0;
	puts("C");
	for(int col = 0; col < cols*pixel_bytes;){
		for(int i = 0; i < pixel_bytes; i ++){
			for(int row = top_rows; row <= bottom_rows; row ++){
				if(mat[row*cols*pixel_bytes+col] != null_values[i]){
					left_cols = col/pixel_bytes;
					col = cols*pixel_bytes;
					i = pixel_bytes;
					break;
				}
			}
			col ++;
		}
	}

	matrix_size_t right_cols = 0;
	//index = 0;
	puts("D");
	for(int col = cols*pixel_bytes-1; col >= 0; ){
		for(int i = pixel_bytes-1; i >= 0; i --){
			for(int row = top_rows; row <= bottom_rows; row ++){
				if(mat[row*cols*pixel_bytes+col] != null_values[i]){
					right_cols = col/pixel_bytes;
					col = -1;
					i = -1;
					break;
				}
			}
			col --;
		}
	}
	puts("DONE");
	*width_start = left_cols;
	*width_stop = right_cols;

	*height_start = top_rows;
	*height_stop = bottom_rows;
}


void matrix_1ubyteMat_calculate_crop_old(int * width_start, int * width_stop, int * height_start, int * height_stop, uint8_t * mat, matrix_size_t rows, matrix_size_t cols){
	matrix_size_t top_rows = 0;
	for(int row = 0; row < rows; row ++){
		for(int col = 0; col < cols; col ++){
			if(mat[row*cols+col] != 0){
				top_rows = row;
				row = rows;
				break;
			}
		}
	}

	matrix_size_t bottom_rows = 0;
	for(int row = rows-1; row >= 0; row --){
		for(int col = 0; col < cols; col ++){
			if(mat[row*cols+col] != 0){
				bottom_rows = row;
				row = -1;
				break;
			}
		}
	}

	matrix_size_t left_cols = 0;
	for(int col = 0; col < cols; col ++){
		for(int row = top_rows; row <= bottom_rows; row ++){
			if(mat[row*cols+col] != 0){
				left_cols = col;
				col = cols;
				break;
			}
		}
	}

	matrix_size_t right_cols = 0;
	for(int col = cols-1; col >= 0; col --){
		for(int row = top_rows; row <= bottom_rows; row ++){
			if(mat[row*cols+col] != 0){
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



uint8_t * matrix_1ubyteMat_crop_edges(matrix_size_t * out_rows, matrix_size_t * out_cols, uint8_t * mat, matrix_size_t rows, matrix_size_t cols){
	uint8_t * out = NULL;
	/*puts("HERE");
	if(out == NULL) puts("null before malloc");
	out = (uint8_t *)malloc(100);
	if(out == NULL) puts("null after malloc");
	out[0] = 69;
	puts("ALLOC done");
	*out_rows = 1;
	*out_cols = 1;
	return out;
	*/

	matrix_size_t top_rows = 0;
	for(int row = 0; row < rows; row ++){
		for(int col = 0; col < cols; col ++){
			if(mat[row*cols+col] != 0){
				top_rows = row;
				row = rows;
				break;
			}
		}
	}

	matrix_size_t bottom_rows = 0;
	for(int row = rows-1; row >= 0; row --){
		for(int col = 0; col < cols; col ++){
			if(mat[row*cols+col] != 0){
				bottom_rows = row;
				row = -1;
				break;
			}
		}
	}

	matrix_size_t left_cols = 0;
	for(int col = 0; col < cols; col ++){
		for(int row = top_rows; row <= bottom_rows; row ++){
			if(mat[row*cols+col] != 0){
				left_cols = col;
				col = cols;
				break;
			}
		}
	}

	matrix_size_t right_cols = 0;
	for(int col = cols-1; col >= 0; col --){
		for(int row = top_rows; row <= bottom_rows; row ++){
			if(mat[row*cols+col] != 0){
				right_cols = col;
				col = -1;
				break;
			}
		}
	}
	printf("top: %d, bottom: %d\n", top_rows, bottom_rows);
	printf("left: %d, right: %d\n", left_cols, right_cols);

	*out_rows = bottom_rows-top_rows+1;
	*out_cols = right_cols-left_cols+1;
	printf("size: %ld\n", (*out_rows)*(*out_cols)*sizeof(uint8_t));
	puts("HERE");
	out = (uint8_t *)malloc((*out_rows)*(*out_cols)*sizeof(uint8_t));
	for(int row = top_rows; row <= bottom_rows; row ++){
		for(int col = left_cols; col <= right_cols; col ++){
			printf("i: %d\n", (row-top_rows)*(*out_cols)+(col-left_cols));
			out[(row-top_rows)*(*out_cols)+(col-left_cols)] = mat[row*cols+col];
		}
	}


	return out;
}


uint8_t * matrix_1ubyteMat_add_frame(matrix_size_t * out_rows, matrix_size_t * out_cols, uint8_t * mat, matrix_size_t rows, matrix_size_t cols, matrix_size_t top_rows, matrix_size_t bottom_rows, matrix_size_t left_cols, matrix_size_t right_cols){
	*out_rows = rows + top_rows + bottom_rows;
	*out_cols = cols + left_cols + right_cols;
	uint8_t * out = (uint8_t *)calloc((*out_rows)*(*out_cols), sizeof(uint8_t));

	for(int row = 0; row < rows; row ++){
		for(int col = 0; col < cols; col ++){
			out[(row+top_rows)*(*out_cols)+left_cols+col] = mat[row*cols+col];
		}
	}

	return out;
}