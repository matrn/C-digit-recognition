#include "include/matrix.h"

int main(){
	matrix_t mat_double;
	matrix_t mat_int;
	matrix_t mat_uint;

	matrix_alloc(&mat_double, MATRIX_DOUBLE, 5, 5);
	matrix_alloc(&mat_int, MATRIX_INT, 3, 5);
	matrix_alloc(&mat_uint, MATRIX_UINT, 3, 5);

	printf("%d\n", matrix_eye(&mat_double));
	
	matrix_print_wh(&mat_double, true);
	matrix_print_wh(&mat_int, true);
	matrix_print_wh(&mat_uint, true);


	matrix_free(&mat_double);
	matrix_free(&mat_int);
	matrix_free(&mat_uint);

	return 0;
}