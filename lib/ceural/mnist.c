#include "include/mnist.h"


/**
 * @brief load MNIST labels into mnist_labels_t structure
 * 
 * @param[out] labels pointer to the mnist_labels_t structure
 * @param[in] filename filename of the MNIST labels file
 * @return mnist_rtn returns MNIST_OK if ok, negative value otherwise (MNIST_FILE_ERROR or MNIST_PARSE_ERROR)
 */
mnist_rtn mnist_labels_load(mnist_labels_t * labels, char * filename){
	int32_t items_num;
	FILE *f;
	labels->data = NULL;

	f = fopen(filename, "r");

	if(f){
		int8_t buf[4];

		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		if(MSB_4bytes_to_int32(buf) != 2049) return MNIST_PARSE_ERROR;   // magic

		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		if((items_num = MSB_4bytes_to_int32(buf)) < 0) return MNIST_PARSE_ERROR;

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

/**
 * @brief frees mnist labels memory
 * 
 * @param[in] labels pointer to the mnist_labels_t structure
 */
void mnist_labels_free(mnist_labels_t * labels){
	free(labels->data);
}

/**
 * @brief loads MNIST images into mnis_images_t structure
 * 
 * @param[out] images pointer to the mnist_images_t structure
 * @param[in] filename filename of MNIST images file
 * @return mnist_rtn returns MNIST_OK if ok, negative value otherwise (MNIST_FILE_ERROR or MNIST_PARSE_ERROR)returns MNIST_OK if ok, negative value otherwise (MNIST_FILE_ERROR or MNIST_PARSE_ERROR)
 */
mnist_rtn mnist_images_load(mnist_images_t* images, char* filename){
	int32_t items_num;
	int32_t rows, cols;

	FILE *f;
	images->data = NULL;

	f = fopen(filename, "r");

	if(f){
		int8_t buf[4];

		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		if(MSB_4bytes_to_int32(buf) != 2051) return MNIST_PARSE_ERROR;   // magic

		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		if((items_num = MSB_4bytes_to_int32(buf)) < 0) return MNIST_PARSE_ERROR;

		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		if((rows = MSB_4bytes_to_int32(buf)) < 0) return MNIST_PARSE_ERROR;

		if(fread(&buf, 4, 1, f) != 1) return MNIST_PARSE_ERROR;
		if((cols = MSB_4bytes_to_int32(buf)) < 0) return MNIST_PARSE_ERROR;


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


/**
 * @brief frees mnist images memory
 * 
 * @param[in] images pointer to the mnist_images_t structure
 */
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


/**
 * @brief convert mnist_labels_t (uint8_t) into matrix_t vector
 * 
 * @param[out] label_mat pointer to the output matrix_t vector
 * @param[in] label input label (number)
 */
void mnist_label_vectorize(matrix_t * label_mat, mnist_label_t label){
	matrix_resize(label_mat, 10, 1);
	matrix_zero(label_mat);
	*matrix_at(label_mat, label, 0) = 1;
}