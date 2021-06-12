#include "include/matrix.h"

int main(){
	matrix_t mat_double;
	matrix_t mat_tinyint;
	matrix_t mat_int;
	matrix_t mat_bigint;

	matrix_alloc(&mat_double, MATRIX_DOUBLE, 5, 5);
	matrix_alloc(&mat_tinyint, MATRIX_TINYINT, 3, 5);
	matrix_alloc(&mat_int, MATRIX_INT, 3, 5);
	matrix_alloc(&mat_bigint, MATRIX_BIGINT, 3, 5);

	printf("%d\n", matrix_eye(&mat_double));
	
	matrix_print_wh(&mat_double, true);
	matrix_print_wh(&mat_tinyint, true);
	matrix_print_wh(&mat_int, true);
	matrix_print_wh(&mat_bigint, true);


	matrix_free(&mat_double);
	matrix_free(&mat_tinyint);
	matrix_free(&mat_int);
	matrix_free(&mat_bigint);

	return 0;
}