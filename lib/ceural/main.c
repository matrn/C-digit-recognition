#include "include/ceural.h"


/*
 * Test example for ceural library
 */

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

	mnist_labels_t test_labels;
	mnist_images_t test_images;

	mnist_labels_load(&train_labels, "../../data/train-labels-idx1-ubyte");
	mnist_images_load(&train_images, "../../data/train-images-idx3-ubyte");
	
	mnist_labels_load(&test_labels, "../../data/t10k-labels-idx1-ubyte");
	mnist_images_load(&test_images, "../../data/t10k-images-idx3-ubyte");

	// printf("size: %ld\n", sizeof(train_images.data[0][1]));
	// printf("data0: %d\n", train_images.data[0][0]);

	
	mnist_set_t train_set = {
		.images = &train_images,
		.labels = &train_labels
	};

	mnist_set_t test_set = {
		.images = &test_images,
		.labels = &test_labels
	};

	matrix_t * out = matrix_new(); //calloc(10, sizeof(double));

	cstart(forward_phase);
	ceural_net_forward(&nn, out, train_images.data[0]);
	cstop(forward_phase);

	puts("Loading NN...");
	ceural_net_load_from_file(&nn, "data.ceural");

	puts("Testing...");
	ceural_net_test(&nn, &test_set);
	

	//ceural_net_backpropagate(&nn, train_images.data[0], train_labels.data[0]);
	ceural_net_train(&nn, &train_set, 1, 32);

	
	ceural_net_forward(&nn, out, train_images.data[0]);
	for(int i = 0; i < 10; i ++) printf("%-8d", i);
	printf("\n");
	for(int i = 0; i < 10; i ++) printf("%-8.3f", matrix_atv(out, i, 0));
	printf("\n");
	matrix_delete(out);

	double accuracy = ceural_net_test(&nn, &test_set);

	puts("Saving...");
	ceural_net_save_to_file(&nn, "data.ceural", accuracy);

	mnist_labels_free(&train_labels);
	mnist_images_free(&train_images);

	mnist_labels_free(&test_labels);
	mnist_images_free(&test_images);

	ceural_net_free(&nn);
	return 0;
}