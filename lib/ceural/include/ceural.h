#ifndef CEURAL_H
#define CEURAL_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../../lag/include/lag.h"
#include "mnist.h"

enum ceural_activation { ACTIVATION_RELU,
						 ACTIVATION_SIGMOID };

typedef struct CeuralLayerDefinition {
	char* name;
	uint16_t input_dim;
	uint16_t output_dim;
	enum ceural_activation activation;
} ceural_layer_definition_t;

typedef struct CeuralNetworkDefinition {
	uint16_t layers;
	ceural_layer_definition_t * layout;
} ceural_net_definition_t;


typedef struct CeuralLayer {
	matrix_t weights;
	matrix_t bias;
	matrix_t * (*activation_function)(matrix_t *);
	matrix_t * (*activation_function_derivative)(matrix_t *);
	
	// memory:
	matrix_t sum;
	matrix_t output;
	matrix_t delta_sum;
	matrix_t grad_sum;

	int t;
} ceural_layer_t;


typedef struct Ceuralnet {
	uint16_t size;   // number of layers
	ceural_layer_t * layers;
} ceural_net_t;


void ceural_net_create(ceural_net_t * nn, ceural_net_definition_t* nn_def);
void ceural_net_free(ceural_net_t * nn);


matrix_t * ceural_relu(matrix_t * z);
matrix_t * ceural_relu_derivative(matrix_t * z);
matrix_t * ceural_sigmoid(matrix_t * z);
matrix_t * ceural_sigmoid_derivative(matrix_t *z);


#endif