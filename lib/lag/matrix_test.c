#include "include/matrix.h"

int main(){
	matrix_t mat_double;
	matrix_t mat_int;
	matrix_t out;
	out.data = NULL;

	matrix_init(&out);

	matrix_alloc(&mat_double, MATRIX_DOUBLE, 5, 5);
	matrix_alloc(&mat_int, MATRIX_INT, 5, 5);

	printf("%d\n", matrix_eye(&mat_double));
	
	matrix_print_wh(&mat_double, true);
	matrix_print_wh(&mat_int, true);

	matrix_add(&out, &mat_int, &mat_int);

	matrix_print_wh(&out, true);

	matrix_free(&mat_double);
	matrix_free(&mat_int);
	matrix_free(&out);

	return 0;
}