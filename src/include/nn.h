#ifndef NN_H
#define NN_H

//#include "../../lib/ceural/include/ceural.h"
#include <ceural.h>

ceural_net_t nn;



#define NN_DATA_FILENAME "../data/data.ceural"

#define MNIST_TRAIN_LABELS "../data/train-labels-idx1-ubyte"
#define MNIST_TRAIN_IMAGES "../data/train-images-idx3-ubyte"

#define MNIST_TEST_LABELS "../data/t10k-labels-idx1-ubyte"
#define MNIST_TEST_IMAGES "../data/t10k-images-idx3-ubyte"




void nn_init();
void nn_train();
double nn_test();
int8_t nn_recognise(uint8_t * img, matrix_t * result);
void nn_free();

#endif