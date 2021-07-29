#include "include/ceural.h"


void ceural_relu(matrix_t * out, const matrix_t * z){
	//matrix_t * out = matrix_new();
	//matrix_alloc(out, z->r, z->c);

	matrix_maximum(out, z, 0);

	//return out;
}

void ceural_relu_derivative(matrix_t * out, const matrix_t * z){
	//matrix_t * out = matrix_new();
	//out->data = NULL;
	matrix_copy(out, z);

	for(int i = 0; i < z->r * z->c; i ++){
		out->data[i] = z->data[i] <= 0 ? 0 : 1;
	}

	//return out;
}

void ceural_sigmoid(matrix_t * out, const matrix_t * z){
	// 1/(1+exp(-z))

	//matrix_t * out = matrix_new();
	//matrix_t * tmp = matrix_new();
	
	matrix_scale(out, z, -1);   // -z
	matrix_exp_ew(out, out);   // exp(-z)
	matrix_add_scalar(out, out, 1);   // 1 + exp(-z)
	matrix_divide_lscalar(out, 1, out);   // 1/(1+exp(-z))

	//matrix_free(tmp);

	//return out;
}


void ceural_sigmoid_derivative(matrix_t * out, const matrix_t * z){
	matrix_t * z_sigmoid = matrix_new();
	ceural_sigmoid(z_sigmoid, z);
	//matrix_t * out = matrix_new();

	matrix_sub_lscalar(out, 1, z_sigmoid);
	matrix_multiply_ew(out, z_sigmoid, out);

	matrix_delete(z_sigmoid);
	//return out;
}