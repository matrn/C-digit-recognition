#ifndef CEURAL_MNIST_H
#define CEURAL_MNIST_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../lag/include/lag.h"

#define MNIST_OK 0
#define MNIST_FILE_ERROR -1
#define MNIST_PARSE_ERROR -2



int32_t MSB_4bytes_to_int(int8_t data[]);


typedef int8_t mnist_rtn;

typedef struct MNISTLabels {
	int32_t length;
	uint8_t* data;
} mnist_labels_t;

mnist_rtn mnist_labels_load(mnist_labels_t* labels, char* filename);
void mnist_labels_free(mnist_labels_t* labels);



typedef uint8_t mnist_pixel_t;

typedef struct MNISTImages {
	int32_t length;
	int32_t rows, cols;
	mnist_pixel_t ** data;
} mnist_images_t;

mnist_rtn mnist_images_load(mnist_images_t* images, char* filename);
void mnist_images_free(mnist_images_t* images);




#endif