#include "include/lag.h"

/**
 * @brief sets the random seed for the random function
 * 
 * @param[in] seed 
 */
void random_seed(unsigned int seed){
	srand(seed);
}

void blas_set_num_threads(int num){
	openblas_set_num_threads(num);
}