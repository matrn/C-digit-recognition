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

#define DOUBLE_SIZE sizeof(double)

union double_cast {
	double value;
	int16_t int16;
	int32_t int32;
	int64_t int64;
	int8_t bytes[DOUBLE_SIZE];
};


void int32_to_MSB_4bytes(int8_t out[], int32_t in);
void int16_to_MSB_2bytes(int8_t out[], int16_t in);
void double_to_MSB_bytes(int8_t out[], double in);


int32_t MSB_4bytes_to_int32(int8_t bytes[]);
int16_t MSB_2bytes_to_int16(int8_t bytes[]);
double MSB_bytes_to_double(int8_t bytes[]);



#endif