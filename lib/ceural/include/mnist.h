#ifndef CEURAL_MNIST_H
#define CEURAL_MNIST_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


#include "../../lag/include/lag.h"
#include "convert.h"


#define MNIST_OK 0
#define MNIST_FILE_ERROR -1
#define MNIST_PARSE_ERROR -2


typedef int8_t mnist_rtn;
typedef uint8_t mnist_pixel_t;
typedef uint8_t mnist_label_t;

typedef struct MNISTLabels {
	int32_t length;
	mnist_label_t* data;
} mnist_labels_t;

mnist_rtn mnist_labels_load(mnist_labels_t* labels, char* filename);
void mnist_labels_free(mnist_labels_t* labels);





typedef struct MNISTImages {
	int32_t length;
	int32_t rows, cols;
	mnist_pixel_t ** data;
} mnist_images_t;

mnist_rtn mnist_images_load(mnist_images_t* images, char* filename);
void mnist_images_free(mnist_images_t* images);


typedef struct MNISTSet {
	mnist_images_t * images;
	mnist_labels_t * labels;
} mnist_set_t;


void mnist_label_vectorize(matrix_t * label_mat, mnist_label_t label);

#endif