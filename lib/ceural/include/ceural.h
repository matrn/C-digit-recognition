#ifndef CEURAL_H
#define CEURAL_H

#include <lag.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "mnist.h"

typedef int8_t ceural_rtn;
#define CEURAL_OK 0
#define CEURAL_FILE_ERROR -1
#define CEURAL_PARSE_ERROR -2

enum ceural_activation { ACTIVATION_RELU,
						 ACTIVATION_SIGMOID };

typedef struct CeuralLayerDefinition {
	char *name;
	uint16_t input_dim;
	uint16_t output_dim;
	enum ceural_activation activation;
} ceural_layer_definition_t;

typedef struct CeuralNetworkDefinition {
	uint16_t layers;
	ceural_layer_definition_t *layout;
} ceural_net_definition_t;

typedef struct CeuralLayer {
	matrix_t weights;
	matrix_t bias;
	void (*activation_function)(matrix_t *, const matrix_t *);
	void (*activation_function_derivative)(matrix_t *, const matrix_t *);
	enum ceural_activation activation_function_enum;

	// memory:
	matrix_t sum;
	matrix_t output;
	matrix_t delta_sum;
	matrix_t grad_sum;

	int t;
} ceural_layer_t;

typedef struct Ceuralnet {
	uint16_t size;	// number of layers
	ceural_layer_t *layers;
} ceural_net_t;

#define EULER 2.71828182845904523536

void ceural_relu(matrix_t *out, const matrix_t *z);
void ceural_relu_derivative(matrix_t *out, const matrix_t *z);
void ceural_sigmoid(matrix_t *out, const matrix_t *z);
void ceural_sigmoid_derivative(matrix_t *out, const matrix_t *z);

void ceural_net_create(ceural_net_t *nn, ceural_net_definition_t *nn_def);
void ceural_net_free(ceural_net_t *nn);
void ceural_net_forward(ceural_net_t *nn, matrix_t *output, mnist_pixel_t *input);
void ceural_net_reset_sums(ceural_net_t *nn);
void ceural_net_backpropagate(ceural_net_t *nn, mnist_pixel_t *input, mnist_label_t expected_output);
void ceural_net_update_weigts(ceural_net_t *nn, double learning_rate, uint16_t batch_size);
void ceural_net_train(ceural_net_t *nn, mnist_set_t *train_set, uint16_t epochs, uint16_t batch_size);
double ceural_net_test(ceural_net_t *nn, mnist_set_t *test_set);

ceural_rtn ceural_net_save_to_file(ceural_net_t *nn, const char *filename, double test_accuracy);
ceural_rtn ceural_net_load_from_file(ceural_net_t *nn, const char *filename);

#endif