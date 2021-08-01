#include "include/ceural.h"


void ceural_net_create(ceural_net_t * nn, ceural_net_definition_t * nn_def){
	nn->size = nn_def->layers;
	nn->layers = (ceural_layer_t *)malloc(nn->size*sizeof(ceural_layer_t));

	random_seed(1234);

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

		nn->layers[i].activation_function_enum = layer->activation;
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


void ceural_net_forward(ceural_net_t * nn, matrix_t * output, mnist_pixel_t * input){
	ceural_layer_t * input_layer = &nn->layers[0];

	// printf("size: %ld\n", sizeof(input[0]));
	// for(int i = 0; i < matrix_get_cols(&input_layer->weights); i ++){
	// 	printf("#%d: %d\n", i, input[i]);
	// }

	//matrix_t * sum = matrix_new();
	cstart(multiply_r1ubyte);
	matrix_multiply_r1ubyteMat(&input_layer->sum, &input_layer->weights, input, matrix_get_cols(&input_layer->weights), 1);    // input_dim, 1
	cstop(multiply_r1ubyte);

	//matrix_print_wh(&input_layer->sum, true);
	cstart(mat_add);
	matrix_add(&input_layer->sum, &input_layer->sum, &input_layer->bias);
	cstop(mat_add);

	cstart(activation);
	input_layer->activation_function(&input_layer->output, &input_layer->sum);
	cstop(activation);
	
	cstart(for_loop);
	for(int i = 1; i < nn->size; i ++){
		ceural_layer_t * layer = &nn->layers[i];
		matrix_multiply(&layer->sum, &layer->weights, &nn->layers[i-1].output);
		matrix_add(&layer->sum, &layer->sum, &layer->bias);
		layer->activation_function(&layer->output, &layer->sum);
	}
	cstop(for_loop);

	cstart(matrix_copy);
	matrix_t * output_mat = &nn->layers[nn->size-1].output;
	matrix_copy(output, output_mat);
	cstop(matrix_copy);
	// for(int i = 0; i < matrix_get_rows(output_mat); i ++){
	// 	output[i] = matrix_atv(output_mat, i, 0);
	// }
}


void ceural_net_reset_sums(ceural_net_t * nn){
	for(int i = 0; i < nn->size; i ++){
		matrix_zero(&nn->layers[i].delta_sum);
		matrix_zero(&nn->layers[i].grad_sum);
	}
}


void ceural_net_backpropagate(ceural_net_t * nn, mnist_pixel_t * input, mnist_label_t expected_output_label){
	ceural_layer_t * output_layer = &nn->layers[nn->size-1];

	matrix_t * expected_output = matrix_new();
	mnist_label_vectorize(expected_output, expected_output_label);
	
	
	matrix_t * output = matrix_new();
	ceural_net_forward(nn, output, input);
	
	matrix_t * output_error = matrix_new();
	matrix_sub(output_error, output, expected_output);

	
	matrix_t * activation_d = matrix_new();
	output_layer->activation_function_derivative(activation_d, &output_layer->sum);
	
	matrix_t * output_delta = matrix_new();
	matrix_multiply_ew(output_delta, output_error, activation_d);
	matrix_add(&output_layer->delta_sum, &output_layer->delta_sum, output_delta);
	
	matrix_t * output_grad = matrix_new();
	matrix_transpose(&nn->layers[nn->size-2].output, &nn->layers[nn->size-2].output);
	matrix_multiply(output_grad, output_delta, &nn->layers[nn->size-2].output);
	matrix_add(&output_layer->grad_sum, &output_layer->grad_sum, output_grad);
	
	matrix_t * delta = matrix_new();
	matrix_t * right_layer_delta = output_delta;
	for(int i = nn->size-2; i >= 0; i --){
		//printf("index: %d\n", i);

		ceural_layer_t * layer = &nn->layers[i];
		matrix_t * right_layer_weights = &nn->layers[i+1].weights;

		matrix_t * error = matrix_new();
		// matrix_print_size(right_layer_weights);
		// matrix_print_size(right_layer_delta);

		matrix_t * weights_copy = matrix_new();
		matrix_transpose(weights_copy, right_layer_weights);
		matrix_multiply(error, weights_copy, right_layer_delta);
		matrix_delete(weights_copy);

		layer->activation_function_derivative(activation_d, &layer->sum);

		matrix_multiply_ew(delta, error, activation_d);
		right_layer_delta = delta;

		matrix_add(&layer->delta_sum, &layer->delta_sum, delta);

		matrix_t * grad = matrix_new();
		// matrix_print_size(delta);
		//matrix_print_size(&nn->layers[i-1].output);
		//matrix_transpose(delta, delta);
		if(i == 0){
			matrix_multiply_r1ubyteMat(grad, delta, input, 1, matrix_get_cols(&nn->layers[0].weights));
		}
		else{
			matrix_multiply(grad, delta, &nn->layers[i-1].output);
		}
		matrix_add(&layer->grad_sum, &layer->grad_sum, grad);

		matrix_delete(error);
		//matrix_delete(activation_d);
		
		matrix_delete(grad);
	}

	matrix_delete(delta);
	matrix_delete(output);
	matrix_delete(expected_output);
	matrix_delete(output_error);
	matrix_delete(output_delta);
	matrix_delete(output_grad);
	matrix_delete(activation_d);	
}


void ceural_net_update_weigts(ceural_net_t * nn, double learning_rate, uint16_t batch_size){
	for(int i = 0; i < nn->size; i ++){
		ceural_layer_t * layer = &nn->layers[i];

		matrix_t * delta = matrix_new();
		matrix_t * grad = matrix_new();

		matrix_divide_rscalar(delta, &layer->delta_sum, batch_size);
		matrix_divide_rscalar(grad, &layer->grad_sum, batch_size);

		//matrix_print_wh(delta, true);
		//matrix_print_wh(grad, true);

		matrix_scale(delta, delta, learning_rate);
		matrix_scale(grad, grad, learning_rate);


		matrix_sub(&layer->weights, &layer->weights, grad);
		matrix_sub(&layer->bias, &layer->bias, delta);


		matrix_delete(delta);
		matrix_delete(grad);
	}
}


void ceural_net_train(ceural_net_t * nn, mnist_set_t * train_set, uint16_t epochs, uint16_t batch_size){
	for(int epoch = 0; epoch < epochs; epoch ++){
		printf("Epoch #%d\n", epoch +1);

		int set_len = train_set->images->length;
		//set_len = 5000;
		puts("");
		for(int i = 0; i < set_len; i ++){
			int batch_fin = i + batch_size;
			ceural_net_reset_sums(nn);
			if(batch_fin > set_len) batch_fin = set_len;

			int batch_count = 0;
			for(; i < batch_fin; i ++){
				//printf("i: %d\n", i);

				ceural_net_backpropagate(nn, train_set->images->data[i], train_set->labels->data[i]);
				batch_count ++;
			}
			ceural_net_update_weigts(nn, 0.001, batch_count);
			printf("\ri: %6d. ", i);
			fflush(stdout);
		}
	}
}

double ceural_net_test(ceural_net_t * nn, mnist_set_t * test_set){
	int correct = 0;
	int wrong = 0;
	printf("Running test set with length %d\n", test_set->images->length);

	int set_len = test_set->images->length;
	matrix_t * output = matrix_new();
	for(int i = 0; i < set_len; i ++){
		ceural_net_forward(nn, output, test_set->images->data[i]);
		mnist_label_t predicted = matrix_argmax(output);
		mnist_label_t expected = test_set->labels->data[i];

		if(predicted == expected) correct += 1;
		else wrong += 1;

		//printf("%d vs %d\n", predicted, expected);
	}
	double accuracy = ((double)correct/set_len)*100.0;
	printf("Correct: %d, Wrong: %d\n", correct, wrong);
	printf("Accuracy: %f %%\n", accuracy);

	matrix_delete(output);

	return accuracy;
}



ceural_rtn ceural_net_save_to_file(ceural_net_t * nn, const char * filename, double train_accuracy){
	/*
	#header #1
		uint16_t, MSB - magic number - 420
		uint16_t, MSB - number of layers
	#layers:
		uint16_t, MSB - input_dim
		uint16_t, MSB - output_dim
		int8_t - activation function
	#header #2
		int8_t - sizeof(double)
		int16_t, MSB - train accuracy (in %) * 100
			
	#layers - matrices:
		## weights:
			uint32_t, MSB - rows
			uint32_t, MSB - cols
			double, MSB - data
		## bias:
			uint32_t, MSB - rows
			uint32_t, MSB - cols
			double, MSB - data
	
	#check:
		double, MSB - euler's number +- 0.0001
	*/
	FILE *f;
	int8_t buf[DOUBLE_SIZE > 4 ? DOUBLE_SIZE : 4];


	f = fopen(filename, "w");
	if(!f) return CEURAL_FILE_ERROR;
	

	

	// magic number
	int16_to_MSB_2bytes(buf, 420);
	fwrite(buf, 2, 1, f);

	// neural network number of layers
	int16_to_MSB_2bytes(buf, nn->size);
	fwrite(buf, 2, 1, f);

	// layers
	for(int i = 0; i < nn->size; i ++){
		ceural_layer_t * layer = &nn->layers[i];

		// input dim
		int16_to_MSB_2bytes(buf, (int16_t)matrix_get_cols(&layer->weights));
		fwrite(buf, 2, 1, f);

		// output dim
		int16_to_MSB_2bytes(buf, (int16_t)matrix_get_rows(&layer->weights));
		fwrite(buf, 2, 1, f);

		// activation function
		int8_t activ_buf = (int8_t)layer->activation_function_enum;
		fwrite(&activ_buf, 1, 1, f);
	}

	// sizeof(deouble)
	int8_t double_size = sizeof(MATRIX_TYPE);
	fwrite(&double_size, 1, 1, f);


	// train accuracy
	int16_to_MSB_2bytes(buf, train_accuracy*100+0.5);
	fwrite(buf, 2, 1, f);

	// weights & biases
	for(int i = 0; i < nn->size; i ++){
		ceural_layer_t * layer = &nn->layers[i];

		/* weights */
		// rows
		int32_to_MSB_4bytes(buf, matrix_get_rows(&layer->weights));
		fwrite(buf, 4, 1, f);

		// cols
		int32_to_MSB_4bytes(buf, matrix_get_cols(&layer->weights));
		fwrite(buf, 4, 1, f);

		// data
		for(int i = 0; i < layer->weights.r*layer->weights.c; i ++){
			double_to_MSB_bytes(buf, layer->weights.data[i]);
			fwrite(buf, DOUBLE_SIZE, 1, f);
		}

		/* bias */
		// rows
		int32_to_MSB_4bytes(buf, matrix_get_rows(&layer->bias));
		fwrite(buf, 4, 1, f);

		// cols
		int32_to_MSB_4bytes(buf, matrix_get_cols(&layer->bias));
		fwrite(buf, 4, 1, f);

		// data
		for(int i = 0; i < layer->bias.r*layer->bias.c; i ++){
			double_to_MSB_bytes(buf, layer->bias.data[i]);
			fwrite(buf, DOUBLE_SIZE, 1, f);
		}
	}

	double_to_MSB_bytes(buf, EULER);
	fwrite(buf, DOUBLE_SIZE, 1, f);


	fclose(f);

	return CEURAL_OK;
}

ceural_rtn ceural_net_load_from_file(ceural_net_t * nn, const char * filename){
	FILE *f;
	int8_t buf[DOUBLE_SIZE > 4 ? DOUBLE_SIZE : 4];
	uint16_t nn_size;

	f = fopen(filename, "r");
	if(!f) return CEURAL_FILE_ERROR;

	// magic number
	if(fread(&buf, 2, 1, f) != 1) return MNIST_PARSE_ERROR;
	if(MSB_2bytes_to_int16(buf) != 420) return MNIST_PARSE_ERROR;

	// neural network number of layers
	if(fread(&buf, 2, 1, f) != 1) return MNIST_PARSE_ERROR;
	nn_size = MSB_2bytes_to_int16(buf);

	printf("nn size: %d\n", nn_size);


	for(int i = 0; i < nn_size; i ++){
		// input dim
		if(fread(&buf, 2, 1, f) != 1) return MNIST_PARSE_ERROR;
		uint16_t input_dim = MSB_2bytes_to_int16(buf);

		// output dim
		if(fread(&buf, 2, 1, f) != 1) return MNIST_PARSE_ERROR;
		uint16_t output_dim = MSB_2bytes_to_int16(buf);

		// activation function
		if(fread(&buf, 1, 1, f) != 1) return MNIST_PARSE_ERROR;
		int8_t activation_function = buf[0];

		printf("in_dim: %d, out_dim: %d, activation: %d\n", input_dim, output_dim, activation_function);
	}

	// sizeof double
	if(fread(&buf, 1, 1, f) != 1) return MNIST_PARSE_ERROR;
	int8_t double_size = buf[0];
	if(double_size != sizeof(MATRIX_TYPE)){
		dbgerr("Different double sizes, unable to parse");
		return MNIST_PARSE_ERROR;
	}

	// train accuracy
	if(fread(&buf, 2, 1, f) != 1) return MNIST_PARSE_ERROR;
	double train_accuracy = MSB_2bytes_to_int16(buf)/100.0;

	printf("Network was tested with accuracy: %.2f %%\n", train_accuracy);


	for(int i = 0; i < nn_size; i ++){
		ceural_layer_t * layer = &nn->layers[i];

		/* weights */
		// rows
		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		uint32_t rows = MSB_4bytes_to_int32(buf);
	
		// cols
		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		uint32_t cols = MSB_4bytes_to_int32(buf);

		//printf("Matrix: %dx%d\n", rows, cols);
		if(rows != layer->weights.r) return MNIST_PARSE_ERROR;
		if(cols != layer->weights.c) return MNIST_PARSE_ERROR;

		for(int i = 0; i < rows*cols; i ++){
			if(fread(&buf, DOUBLE_SIZE, 1, f) != 1) return MNIST_PARSE_ERROR;
			layer->weights.data[i] = MSB_bytes_to_double(buf);
		}
		
		/* bias */
		// rows
		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		rows = MSB_4bytes_to_int32(buf);
		
		// cols
		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		cols = MSB_4bytes_to_int32(buf);

		//printf("Matrix: %dx%d\n", rows, cols);
		if(rows != layer->bias.r) return MNIST_PARSE_ERROR;
		if(cols != layer->bias.c) return MNIST_PARSE_ERROR;

		for(int i = 0; i < rows*cols; i ++){
			if(fread(&buf, DOUBLE_SIZE, 1, f) != 1) return MNIST_PARSE_ERROR;
			layer->bias.data[i] = MSB_bytes_to_double(buf);
		}
	}

	if(fread(&buf, DOUBLE_SIZE, 1, f) != 1) return MNIST_PARSE_ERROR;
	double euler = MSB_bytes_to_double(buf);
	printf("Euler's number: %f\n", euler);

	if(fabs(EULER-euler) > 0.0001) dbgerrln("Something is wrong, I can feel it. e is incorrect");

	
	fclose(f);

	return CEURAL_OK;
}