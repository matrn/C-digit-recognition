#include "include/matrix.h"

int main(){
	matrix_t mat1;
	matrix_alloc(&mat1, MATRIX_DOUBLE, 3, 5);

	matrix_print_wh(&mat1, true);
	matrix_free(&mat1);
	return 0;
}