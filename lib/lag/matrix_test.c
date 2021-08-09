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

	puts("---------------------SubMat move---------------------");
	{
	uint8_t mat[] = {
		6, 0, 8, 0, 0, 1,
		0, 9, 0, 7, 2, 0,
		4, 0, 55, 54, 0, 3,
		0, 3, 53, 52, 4, 0,
		2, 0, 3, 0, 0, 9,
		0, 1, 0, 4, 6, 0,
	};

	int rows = 6;
	int cols = 6;

	matrix_1ubyteMat_print(mat, rows, cols);
	puts("-----------------");
	matrix_1ubyteMat_submat_move(mat, rows, cols, 2, 2, 2, 2, -2, -1);
	matrix_1ubyteMat_print(mat, rows, cols);

	}
	
	return 0;
}