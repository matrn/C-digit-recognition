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
	
	puts("------------------------------------------");

	matrix_resize(&mat, 3, 2);
	MATRIX_TYPE data[] = {
		1, 2,
		3, 4,
		5, 6
	};
	free(mat.data);
	mat.data = &data[0];
	matrix_print_wh(&mat, true);
	matrix_transpose(&out, &mat);
	matrix_print_wh(&out, true);

	//matrix_free(&mat);
	matrix_free(&out);

	return 0;
}