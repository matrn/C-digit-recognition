#include "include/ceural.h"


matrix_t in, out;

void relu(){
	in.data[0] = 2;
	in.data[1] = 2;
	ceural_relu(&out, &in);
	matrix_print(&out);

	in.data[0] = 0;
	in.data[1] = 0;
	ceural_relu(&out, &in);
	matrix_print(&out);

	in.data[0] = -1;
	in.data[1] = -1;
	ceural_relu(&out, &in);
	matrix_print(&out);
}

void relu_d(){
	in.data[0] = 2;
	in.data[1] = 2;
	ceural_relu_derivative(&out, &in);
	matrix_print(&out);

	in.data[0] = 0;
	in.data[1] = 0;
	ceural_relu_derivative(&out, &in);
	matrix_print(&out);

	in.data[0] = -1;
	in.data[1] = -1;
	ceural_relu_derivative(&out, &in);
	matrix_print(&out);
}



void sigmoid(){
	in.data[0] = -1;
	in.data[1] = -2;
	ceural_sigmoid(&out, &in);
	matrix_print(&out);

	in.data[0] = -1;
	in.data[1] = -2;
	ceural_sigmoid(&out, &in);
	matrix_print(&out);

	in.data[0] = 1;
	in.data[1] = 0;
	ceural_sigmoid(&out, &in);
	matrix_print(&out);

	in.data[0] = 0;
	in.data[1] = 1;
	ceural_sigmoid(&out, &in);
	matrix_print(&out);

	in.data[0] = 2;
	in.data[1] = 2;
	ceural_sigmoid(&out, &in);
	matrix_print(&out);
}



void sigmoid_d(){
	in.data[0] = -2;
	in.data[1] = -1;
	ceural_sigmoid_derivative(&out, &in);
	matrix_print(&out);

	in.data[0] = -1;
	in.data[1] = -2;
	ceural_sigmoid_derivative(&out, &in);
	matrix_print(&out);

	in.data[0] = 0;
	in.data[1] = 1;
	ceural_sigmoid_derivative(&out, &in);
	matrix_print(&out);

	in.data[0] = 1;
	in.data[1] = 0;
	ceural_sigmoid_derivative(&out, &in);
	matrix_print(&out);

	in.data[0] = 2;
	in.data[1] = 2;
	ceural_sigmoid_derivative(&out, &in);
	matrix_print(&out);
}

int main(){
	puts("Starting activations test");

	
	matrix_init(&in);
	matrix_init(&out);

	matrix_alloc(&in, 2, 1);

	puts("RELU:");
	relu();

	puts("RELU_d:");
	relu_d();

	puts("SIGMOID:");
	sigmoid();

	puts("SIGMOID_d:");
	sigmoid_d();
	

	matrix_free(&in);
	matrix_free(&out);

	return 0;
}