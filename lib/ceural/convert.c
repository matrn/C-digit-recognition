#include "include/convert.h"

void int32_to_MSB_4bytes(int8_t out[], int32_t in){
	union int32_cast num;
	num.value = IS_BIG_ENDIAN ? in : __bswap_32(in);

	memcpy(out, &num.bytes, sizeof(num.value));
}

void int16_to_MSB_2bytes(int8_t out[], int16_t in){
	union int16_cast num;
	num.value = IS_BIG_ENDIAN ? in : __bswap_16(in);

	memcpy(out, &num.bytes, sizeof(num.value));
}

void double_to_MSB_bytes(int8_t out[], double in){
	union double_cast num;
	num.value = in;

	if(DOUBLE_SIZE == 2) num.int16 = __bswap_16(num.int16);
	else if(DOUBLE_SIZE == 4) num.int32 = __bswap_32(num.int32);
	else if(DOUBLE_SIZE == 8) num.int64 = __bswap_64(num.int64);
	
	memcpy(out, &num.bytes, DOUBLE_SIZE);
}



int32_t MSB_4bytes_to_int32(int8_t bytes[]){
	union int32_cast num;
	memcpy(&num.bytes, bytes, sizeof(num.value));

	if(IS_BIG_ENDIAN) return num.value;
	else return __bswap_32(num.value);
}

int16_t MSB_2bytes_to_int16(int8_t bytes[]){
	union int16_cast num;
	memcpy(&num.bytes, bytes, sizeof(num.value));

	if(IS_BIG_ENDIAN) return num.value;
	else return __bswap_16(num.value);
}

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