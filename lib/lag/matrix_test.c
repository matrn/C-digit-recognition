#include "include/matrix.h"

int main(){
	matrix_t mat;
	matrix_t out;

	matrix_init(&out);
	matrix_alloc(&mat, 5, 5);

	printf("%d\n", matrix_eye(&mat));
	
	matrix_print_wh(&mat, true);

	matrix_add(&out, &mat, &mat);
	matrix_print_wh(&out, true);

	matrix_zero(&out);
	
	matrix_multiply(&out, &mat, &mat);
	matrix_print_wh(&out, true);

	matrix_free(&mat);
	matrix_free(&out);

	return 0;
}