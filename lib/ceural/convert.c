#include "include/convert.h"

/**
 * @brief converts int32_t integer into 4 bytes array in MSB order
 * 
 * @param[out] out pointer to the output int8_t aray
 * @param[in] in int32_t input
 */
void int32_to_MSB_4bytes(int8_t out[], int32_t in){
	union int32_cast num;
	num.value = IS_BIG_ENDIAN ? in : __bswap_32(in);

	memcpy(out, &num.bytes, sizeof(num.value));
}

/**
 * @brief converts int16_t integer into 2 bytes array in MSB order
 * 
 * @param[out] out pointer to the output int8_t aray
 * @param[in] in int16_t input
 */
void int16_to_MSB_2bytes(int8_t out[], int16_t in){
	union int16_cast num;
	num.value = IS_BIG_ENDIAN ? in : __bswap_16(in);

	memcpy(out, &num.bytes, sizeof(num.value));
}

/**
 * @brief converts double into 8 bytes array in MSB order (eventually 2 pr 4 but in this case it's not memory effective)
 * 
 * @param[out] out pointer to the output int8_t array
 * @param[in] in double input
 */
void double_to_MSB_bytes(int8_t out[], double in){
	union double_cast num;
	num.value = in;

	if(DOUBLE_SIZE == 2) num.int16 = __bswap_16(num.int16);
	else if(DOUBLE_SIZE == 4) num.int32 = __bswap_32(num.int32);
	else if(DOUBLE_SIZE == 8) num.int64 = __bswap_64(num.int64);
	
	memcpy(out, &num.bytes, DOUBLE_SIZE);
}


/**
 * @brief converts 4 bytes array in MSB order into int32_t integer
 * 
 * @param[in] bytes pointer to the array of int8_t bytes in MSB order
 * @return int32_t returns int32_t integer
 */
int32_t MSB_4bytes_to_int32(int8_t bytes[]){
	union int32_cast num;
	memcpy(&num.bytes, bytes, sizeof(num.value));

	if(IS_BIG_ENDIAN) return num.value;
	else return __bswap_32(num.value);
}

/**
 * @brief converts 2 bytes array in MSB order into int16_t integer
 * 
 * @param[in] bytes pointer to the array of int8_t bytes in MSB order
 * @return int16_t returns int16_t integer
 */
int16_t MSB_2bytes_to_int16(int8_t bytes[]){
	union int16_cast num;
	memcpy(&num.bytes, bytes, sizeof(num.value));

	if(IS_BIG_ENDIAN) return num.value;
	else return __bswap_16(num.value);
}

/**
 * @brief convert 8 bytes array in MSB order into double
 * 
 * @param[in] bytes pointer to the array of int8_t bytes in MSB order
 * @return double returns double
 */
double MSB_bytes_to_double(int8_t bytes[]){
	union double_cast num;
	memcpy(&num.bytes, bytes, DOUBLE_SIZE);

	if(!IS_BIG_ENDIAN){
		if(DOUBLE_SIZE == 2) num.int16 = __bswap_16(num.int16);
		else if(DOUBLE_SIZE == 4) num.int32 = __bswap_32(num.int32);
		else if(DOUBLE_SIZE == 8) num.int64 = __bswap_64(num.int64);
	}
	return num.value;
}