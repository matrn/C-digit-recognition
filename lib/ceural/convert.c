#include "include/convert.h"


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