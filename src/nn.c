#include "include/nn.h"

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
	}};

ceural_net_definition_t nn_def = {
	.layers = 2,
	.layout = nn_layout};

void nn_init() {
	ceural_net_create(&nn, &nn_def);

	puts("Loading NN...");
	if (ceural_net_load_from_file(&nn, NN_DATA_FILENAME) != CEURAL_OK) nn_train();
}


void nn_train() {
	mnist_labels_t train_labels;
	mnist_images_t train_images;

	if(mnist_labels_load(&train_labels, MNIST_TRAIN_LABELS) != MNIST_OK) return;
	if(mnist_images_load(&train_images, MNIST_TRAIN_IMAGES) != MNIST_OK) return;

	// printf("size: %ld\n", sizeof(train_images.data[0][1]));
	// printf("data0: %d\n", train_images.data[0][0]);

	mnist_set_t train_set = {
		.images = &train_images,
		.labels = &train_labels};

	puts("Training...");
	ceural_net_train(&nn, &train_set, 1, 32);

	double accuracy = nn_test();

	puts("Saving...");
	ceural_net_save_to_file(&nn, NN_DATA_FILENAME, accuracy);

	mnist_labels_free(&train_labels);
	mnist_images_free(&train_images);
}

double nn_test() {
	mnist_labels_t test_labels;
	mnist_images_t test_images;

	mnist_labels_load(&test_labels, MNIST_TEST_LABELS);
	mnist_images_load(&test_images, MNIST_TEST_IMAGES);

	mnist_set_t test_set = {
		.images = &test_images,
		.labels = &test_labels};

	puts("Testing...");
	double accuracy = ceural_net_test(&nn, &test_set);

	mnist_labels_free(&test_labels);
	mnist_images_free(&test_images);

	return accuracy;
}

int8_t nn_recognise(uint8_t* img, matrix_t *out) {
	//matrix_t* out = matrix_new();  //calloc(10, sizeof(double));

	ceural_net_forward(&nn, out, img);
	for (int i = 0; i < 10; i++) printf("%-8d", i);
	printf("\n");
	for (int i = 0; i < 10; i++) printf("%-8.3f", matrix_atv(out, i, 0));
	printf("\n");
	uint8_t result = matrix_argmax(out);
	//*accuracy = matrix_max(out);
	//matrix_delete(out);

	return result;
}

void nn_free() {
	ceural_net_free(&nn);
}