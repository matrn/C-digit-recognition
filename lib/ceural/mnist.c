#include "include/mnist.h"

int32_t MSB_4bytes_to_int(int8_t data[]){
	// source: https://stackoverflow.com/a/12792056/7351855
	volatile uint32_t i=0x01234567;
    // return 0 for big endian, 1 for little endian.
    if((*((uint8_t*)(&i))) == 0x67){
		return (unsigned char)(data[3]) | (unsigned char)(data[2]) << 8 | (unsigned char)(data[1]) << 16 | (unsigned char)(data[0]) << 24;
	}else{
		return (unsigned char)(data[0]) | (unsigned char)(data[1]) << 8 | (unsigned char)(data[2]) << 16 | (unsigned char)(data[3]) << 24;
	}
}


mnist_rtn mnist_labels_load(mnist_labels_t * labels, char * filename){
	int32_t items_num;
	FILE *f;
	labels->data = NULL;

	f = fopen(filename, "r");

	if(f){
		int8_t buf[4];

		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		if(MSB_4bytes_to_int(buf) != 2049) return MNIST_PARSE_ERROR;   // magic

		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		if((items_num = MSB_4bytes_to_int(buf)) < 0) return MNIST_PARSE_ERROR;

		labels->length = items_num;
		labels->data = (uint8_t *)malloc(items_num*sizeof(uint8_t));

		for(int i = 0; i < items_num; i ++){
			if(fread(&labels->data[i], 1, 1, f) != 1){
				fclose(f);
				return MNIST_PARSE_ERROR;
			}
		}

		fclose(f);
		return MNIST_OK;
	}

	return MNIST_FILE_ERROR;
}


void mnist_labels_free(mnist_labels_t * labels){
	free(labels->data);
}


mnist_rtn mnist_images_load(mnist_images_t* images, char* filename){
	int32_t items_num;
	int32_t rows, cols;

	FILE *f;
	images->data = NULL;

	f = fopen(filename, "r");

	if(f){
		int8_t buf[4];

		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		if(MSB_4bytes_to_int(buf) != 2051) return MNIST_PARSE_ERROR;   // magic

		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		if((items_num = MSB_4bytes_to_int(buf)) < 0) return MNIST_PARSE_ERROR;

		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		if((rows = MSB_4bytes_to_int(buf)) < 0) return MNIST_PARSE_ERROR;

		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		if((cols = MSB_4bytes_to_int(buf)) < 0) return MNIST_PARSE_ERROR;


		images->length = items_num;
		images->data = (mnist_pixel_t **)malloc(items_num*sizeof(mnist_pixel_t *));

		for(int i = 0; i < items_num; i ++){
			images->data[i] = (mnist_pixel_t *)malloc(rows*cols*sizeof(mnist_pixel_t));

			if(fread(images->data[i], sizeof(mnist_pixel_t), rows*cols, f) != rows*cols){
				fclose(f);
				return MNIST_PARSE_ERROR;
			}
		}

		fclose(f);
		return MNIST_OK;
	}

	return MNIST_FILE_ERROR;
}



void mnist_images_free(mnist_images_t* images){
	if(images->data == NULL) return;

	// for(int i = 0; i < images->length; i ++){
	// 	matrix_free(&images->data[i]);
	// }
	for(int i = 0; i < images->length; i ++){
		free(images->data[i]);
	}

	free(images->data);
}



void mnist_label_vectorize(matrix_t * label_mat, mnist_label_t label){
	matrix_resize(label_mat, 10, 1);
	matrix_zero(label_mat);
	*matrix_at(label_mat, label, 0) = 1;
}