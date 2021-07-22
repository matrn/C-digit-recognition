#include "include/ceural.h"


void ceural_net_create(ceural_net_t * nn, ceural_net_definition_t * nn_def){
	nn->size = nn_def->layers;
	nn->layers = (ceural_layer_t *)malloc(nn->size*sizeof(ceural_layer_t));

	random_seed(123);


	for(int i = 0; i < nn_def->layers; i ++){
		ceural_layer_definition_t * layer = &nn_def->layout[i];
		printf("#%d: %s - %d->%d, %d\n", i, layer->name, layer->input_dim, layer->output_dim, layer->activation);

		matrix_alloc(&nn->layers[i].weights, layer->output_dim, layer->input_dim);
		matrix_random_uniform(&nn->layers[i].weights, -1, 1);
		//printf("%f", sqrt(layer->input_dim*layer->output_dim));
		matrix_divide_rscalar(&nn->layers[i].weights, &nn->layers[i].weights, sqrt(layer->input_dim*layer->output_dim));
		//matrix_print_wh(&nn->layers[i].weights, true);

		matrix_alloc(&nn->layers[i].bias, layer->output_dim, 1);
		
		matrix_alloc(&nn->layers[i].sum, layer->output_dim, 1);
		matrix_alloc(&nn->layers[i].output, layer->output_dim, 1);
		matrix_alloc(&nn->layers[i].delta_sum, layer->output_dim, 1);
		matrix_alloc(&nn->layers[i].grad_sum, layer->output_dim, layer->input_dim);

		switch(layer->activation){
			case ACTIVATION_RELU:
				nn->layers[i].activation_function = &ceural_relu;
				nn->layers[i].activation_function_derivative = &ceural_relu_derivative;
				break;

			case ACTIVATION_SIGMOID:
				nn->layers[i].activation_function = &ceural_sigmoid;
				nn->layers[i].activation_function_derivative = &ceural_sigmoid_derivative;
				break;
		}
	}

}


void ceural_net_free(ceural_net_t * nn){
	for(int i = 0; i < nn->size; i ++){
		ceural_layer_t * layer = &nn->layers[i];
		matrix_free(&layer->weights);
		matrix_free(&layer->bias);
		matrix_free(&layer->sum);
		matrix_free(&layer->output);
		matrix_free(&layer->delta_sum);
		matrix_free(&layer->grad_sum);
	}

	free(nn->layers);
}


void ceural_net_forward(MATRIX_TYPE * output, ceural_net_t * nn, mnist_pixel_t * input){
	ceural_layer_t * input_layer = &nn->layers[0];

	// printf("size: %ld\n", sizeof(input[0]));
	// for(int i = 0; i < matrix_get_cols(&input_layer->weights); i ++){
	// 	printf("#%d: %d\n", i, input[i]);
	// }

	//matrix_t * sum = matrix_new();
	matrix_multiply_r1ubyteMat(&input_layer->sum, &input_layer->weights, input, matrix_get_cols(&input_layer->weights), 1);    // input_dim, 1
	//matrix_print_wh(&input_layer->sum, true);
	matrix_add(&input_layer->sum, &input_layer->sum, &input_layer->bias);
	input_layer->activation_function(&input_layer->output, &input_layer->sum);

	for(int i = 1; i < nn->size; i ++){
		ceural_layer_t * layer = &nn->layers[i];
		matrix_multiply(&layer->sum, &layer->weights, &nn->layers[i-1].output);
		matrix_add(&layer->sum, &layer->sum, &layer->bias);
		layer->activation_function(&layer->output, &layer->sum);
	}

	matrix_t * output_mat = &nn->layers[nn->size-1].output;
	for(int i = 0; i < matrix_get_rows(output_mat); i ++){
		output[i] = matrix_atv(output_mat, i, 0);
	}

}