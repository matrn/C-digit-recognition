#include "include/ceural.h"


void ceural_net_create(ceural_net_t * nn, ceural_net_definition_t * nn_def){
	nn->size = nn_def->layers;
	nn->layers = (ceural_layer_t *)malloc(nn->size*sizeof(ceural_layer_t));

	for(int i = 0; i < nn_def->layers; i ++){
		ceural_layer_definition_t * layer = &nn_def->layout[i];
		printf("#%d: %s - %d->%d, %d\n", i, layer->name, layer->input_dim, layer->output_dim, layer->activation);

		matrix_alloc(&nn->layers[i].weights, layer->output_dim, layer->input_dim);
		matrix_alloc(&nn->layers[i].bias, layer->output_dim, 1);
		
		matrix_alloc(&nn->layers[i].sum, layer->input_dim, 1);
		matrix_alloc(&nn->layers[i].output, layer->input_dim, 1);
		matrix_alloc(&nn->layers[i].delta_sum, layer->output_dim, 1);
		matrix_alloc(&nn->layers[i].grad_sum, layer->output_dim, layer->input_dim);

		
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