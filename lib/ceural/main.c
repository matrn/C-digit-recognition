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


	mnist_labels_t train_labels;
	mnist_images_t train_images;

	mnist_labels_load(&train_labels, "../../data/train-labels-idx1-ubyte");
	printf("rtn: %d\n", mnist_images_load(&train_images, "../../data/train-images-idx3-ubyte"));
	
	// printf("size: %ld\n", sizeof(train_images.data[0][1]));
	// printf("data0: %d\n", train_images.data[0][0]);

	double * out = calloc(10, sizeof(double));
	ceural_net_forward(out, &nn, train_images.data[0]);
	for(int i = 0; i < 10; i ++) printf("%-8d", i);
	printf("\n");
	for(int i = 0; i < 10; i ++) printf("%-8.3f", out[i]);
	printf("\n");
	free(out);

	mnist_labels_free(&train_labels);
	mnist_images_free(&train_images);

	ceural_net_free(&nn);
	return 0;
}