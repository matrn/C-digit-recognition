#ifndef CONVERT_H
#define CONVERT_H

#include <byteswap.h>
#include <stdint.h>
#include <string.h>


#define IS_BIG_ENDIAN (!*(unsigned char *)&(uint16_t){1})

union int32_cast {
	int32_t value;
	int8_t bytes[4];
};

union int16_cast {
	int16_t value;
	int8_t bytes[4];
};


int32_t MSB_4bytes_to_int32(int8_t data[]);
int16_t MSB_2bytes_to_int16(int8_t data[]);

void int32_to_MSB_4bytes(int8_t out[], int32_t in);
void int16_to_MSB_2bytes(int8_t out[], int16_t in);

#endif