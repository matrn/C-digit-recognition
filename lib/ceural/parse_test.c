#include "include/ceural.h"



int main(){
	ceural_net_t nn;

	ceural_layer_definition_t nn_layout[] = {
		{
			.name = "hidden layer",
			.input_dim = 784,
			.output_dim = 128,
			.activation = ACTIVATION_RELU,
		},
		{
			.name = "output layer",
			.input_dim = 128,
			.output_dim = 10,
			.activation = ACTIVATION_SIGMOID,
		}
	};
	ceural_net_definition_t nn_def = {
		.layers = 2,
		.layout = nn_layout
	};

	ceural_net_create(&nn, &nn_def);

	puts("---------------");
	printf("save rtn: %d\n", ceural_net_save_to_file(&nn, "data.ceural"));
	printf("load rtn: %d\n", ceural_net_load_from_file(&nn, "data.ceural"));

	ceural_net_free(&nn);
	return 0;
}