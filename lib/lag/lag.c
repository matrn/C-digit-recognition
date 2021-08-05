#include "include/lag.h"

/**
 * @brief sets the random seed for the random function
 * 
 * @param[in] seed 
 */
void random_seed(unsigned int seed){
	srand(seed);
}