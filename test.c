/* 
   COPYRIGHT NOTICE  
   Copyright (C) 2020, CovHappy, All Rights Reserved
   Author: CovHappy(CovHappy@163.com)
   
   https://github.com/CovHappy/hibuf
    
   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published
   by the Free Software Foundation; either version 2.1 of the License, 
   or (at your option) any later version.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hibuf.h"

typedef struct __elem__ {
	int8_t s8;
	int16_t s16;
	int32_t s32;
	int64_t s64;
	char    str[4];
} elem_t;
typedef HIBUF_ARRAY_HEAD(__elem__array, __elem__) elem_list;

typedef struct __object__ {
	uint8_t  u8;	
	uint16_t u16;
	uint32_t u32;
	uint64_t u64;
	elem_t   object;
	elem_list array;
	float   fl;
	double  dou;	
	int32_t z32;
	int64_t z64;
	time_t  time;
} object_t;

void binarydump(unsigned char * buffer, int size)
{
	int n, m;
	uint8_t bits;

	if ( NULL == buffer || 0 == size ) {
		return;
	}

	printf("[");
	for( n=size-1; n>=0; --n ) {
		bits = buffer[n];
		m = 7;
		while( m >= 0 ) {
			if ( (bits >> m--) & 0x01 ) {
				printf("1");	
			} else {
				printf("0");
			}
		}
		printf(" ");
	}
	printf("]");
}
void hexdump(unsigned char * buffer, int size)
{
	int n;
	if ( NULL == buffer || 0 == size ) {
		return;
	}

	for (n=0; n<size; ++n) {
		//tail \n
		if ( (n+1)%16 == 0 ) putchar('\n');
		printf("%02X ", buffer[n]);	
	}
	if ( (n+1)%16 != 0 ) {
		putchar('\n');
	}
}


//define struct
static elem_t array[2] = {{1, 0x00FF, 0x0000FFFF, 0x00000000FFFFFFFF, "123"}, {0xFF, 0xFFFF, 0xFFFFFFFF, 0xFFFFFFFFFFFFFFFF, "1234"}};
static object_t object = {
		.u8 = 1, 
		.u16 = 2, 
		.u32 = 3, 
		.u64 = 4, 
		.fl = 1.1234567,//5-6 number
		.dou = 1234567890.1234567,//17 number
		.z32 = -1,
		.z64 = -2147483647,
		.time = 123456790,
		.object = {
			.s8 = 5,
			.s16 = 6,
			.s32 = 7,
			.s64 = 8,
		},
		.array = {
			.count = sizeof(array)/sizeof(array[0]),
			.data  = array,
		}
	};

static hibuf_meta_t meta_object_t, meta_elem_t;
void init()
{
	//setup hibuf meta
	HIBUF_MAPPING(meta_elem_t, elem_t,
		HIBUF_FIELD_S8(7, s8),
		HIBUF_FIELD_S16(8, s16),
		HIBUF_FIELD_S32(9, s32),
		HIBUF_FIELD_S64(10, s64),
		HIBUF_FIELD_STRING(11,str),
	);
	HIBUF_MAPPING(meta_object_t, object_t,
		HIBUF_FIELD_U8(1, u8),
		HIBUF_FIELD_U16(2, u16),
		HIBUF_FIELD_U32(3, u32),
		HIBUF_FIELD_U64(4, u64),
		//HIBUF_FIELD_OBJECT(5, object, &meta_elem_t),
		HIBUF_FIELD_ARRAY(6, array, &meta_elem_t),
		HIBUF_FIELD_FLOAT(7, fl),
		HIBUF_FIELD_DOUBLE(8, dou),
		HIBUF_FIELD_Z32(9, z32),
		HIBUF_FIELD_Z64(10, z64),
		HIBUF_FIELD_TIME(11, time),
	);	
}
	//get buffer length of encode
void test_hibuf_with_buffer()
{
	int n = 0, byte_size = 0, size = 0;
	char * buffer = NULL;
	object_t * obj_ptr = NULL;
	elem_t   * elem_ptr = NULL;

	//get buffer length of encode
	byte_size = hibuf_byte_length(&meta_object_t, &object);
	printf("hibuf_byte_length(meta %p, object %p) = byte_size %d\n\n", 
		&meta_object_t, &object, byte_size);
	
	//calloc space
	buffer = calloc(1, byte_size);

	//encode data
	printf("##ENCODE DATA:");
	size = hibuf_encode(&meta_object_t, &object, buffer, byte_size);	
	printf("\nhibuf_encode(meta %p, object %p, buffer %p, byte_size %d)= size %d\n\n", 
		&meta_object_t, &object, buffer, byte_size, size);

	//printf hex formmat
	printf("hex dump:\n");
	hexdump(buffer, size);

	//decode data
	printf("\n##DECODE DATA:\n");
	int ret = hibuf_decode(&meta_object_t, buffer, size, (void **)&obj_ptr);
	if ( ret < 0 ) {
		printf("hibuf_decode(meta %p, bytebuffer %p, size %d) = %d, object %p\n", 
				&meta_object_t, buffer, size, ret, obj_ptr);	
		goto LABEL_FREE;
	}

	//printf result after decode
	printf("object attribute: u8 %02X, u16 %02X, u32 %02X, u64 %02X, "
		"fl %f, double %lf, z32 %d, z64 %d, time %ld\n", 
		obj_ptr->u8, obj_ptr->u16, obj_ptr->u32, obj_ptr->u64, 
		obj_ptr->fl, obj_ptr->dou, obj_ptr->z32, obj_ptr->z64,
		obj_ptr->time);
	printf("object array    : count %d\n", obj_ptr->array.count);
	for ( n=0, elem_ptr = obj_ptr->array.data; 
		n<obj_ptr->array.count; ++n, ++elem_ptr) {
		//if overflow, auto convert [signed int]
		printf("%3d, s8 %010X, s16 %010X, s32 %010X, s64 %018lX, str %s(%d)\n", 
			n, elem_ptr->s8, elem_ptr->s16, elem_ptr->s32, elem_ptr->s64,
			elem_ptr->str, strlen(elem_ptr->str));
	}
	
	//free object
	hibuf_free(&meta_object_t, obj_ptr);

LABEL_FREE:
	//free space
	free(buffer);
}

void test_hibuf()
{
	int n = 0, byte_size = 0, size = 0;
	char * buffer = NULL;
	object_t * obj_ptr = NULL;
	elem_t   * elem_ptr = NULL;

	//encode data
	printf("##ENCODE DATA:");
	size = hibuf_byte_encode(&meta_object_t, &object, &buffer);	
	printf("\nhibuf_encode(meta %p, object %p, buffer %p)= size %d\n\n", 
		&meta_object_t, &object, buffer,  size);

	//printf hex formmat
	printf("hex dump:\n");
	hexdump(buffer, size);

	//decode data
	printf("\n##DECODE DATA:\n");
	int ret = hibuf_decode(&meta_object_t, buffer, size, (void **)&obj_ptr);
	if ( ret < 0 ) {
		printf("hibuf_decode(meta %p, bytebuffer %p, size %d) = %d, object %p\n", 
				&meta_object_t, buffer, size, ret, obj_ptr);	
		goto LABEL_FREE;
	}

	//printf result after decode
	printf("object attribute: u8 %02X, u16 %02X, u32 %02X, u64 %02X, "
		"fl %f, double %lf, z32 %d, z64 %d, time %ld\n", 
		obj_ptr->u8, obj_ptr->u16, obj_ptr->u32, obj_ptr->u64, 
		obj_ptr->fl, obj_ptr->dou, obj_ptr->z32, obj_ptr->z64,
		obj_ptr->time);
	printf("object array    : count %d\n", obj_ptr->array.count);
	for ( n=0, elem_ptr = obj_ptr->array.data; 
		n<obj_ptr->array.count; ++n, ++elem_ptr) {
		//if overflow, auto convert [signed int]
		printf("%3d, s8 %010X, s16 %010X, s32 %010X, s64 %018lX, str %s(%d)\n", 
			n, elem_ptr->s8, elem_ptr->s16, elem_ptr->s32, elem_ptr->s64,
			elem_ptr->str, strlen(elem_ptr->str));
	}

	//free object
	hibuf_free(&meta_object_t, obj_ptr);
	
LABEL_FREE:
	//free space
	if ( buffer ) hibuf_byte_free(buffer);
}

int main()
{
	init();
	test_hibuf();
	//test_hibuf_with_buffer();
	return 0;	
}
