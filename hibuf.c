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

#include "hibuf.h"

void HIBUF_CHECK(const hibuf_meta_t *meta, const char *N, const char *F, const char *M, const long L)
{
	int n = 0;
	const hibuf_field_t * field;
	HIBUF_FIELD_FOREACH(meta, n , field) {
		field->opt->check(meta, field, N, F, M, L);	
	}
}
uint16_t _HIBUF_BSWAP16(uint16_t x, uint8_t force)
{
	uint16_t value = 0;

	if ( force || !HIBUF_BIG_ENDIAN ) {
		value |= (x & 0xFF00UL) >> 8;
		value |= (x & 0x00FFUL) << 8;
	} else {
		value = x;
	}

	return value;
}

uint32_t _HIBUF_BSWAP32(uint32_t x, uint8_t force)
{
	uint64_t value = 0;

	if ( force || !HIBUF_BIG_ENDIAN ) {
		value |= (x & 0xFF000000UL) >> 24;
		value |= (x & 0x00FF0000UL) >> 8;
		value |= (x & 0x0000FF00UL) << 8;
		value |= (x & 0x000000FFUL) << 24;
	} else {
		value = x;
	}

	return value;
}

uint64_t _HIBUF_BSWAP64(uint64_t x, uint8_t force)
{
	uint64_t value = 0;

	if ( force || !HIBUF_BIG_ENDIAN ) {
		value |= (x & 0xFF00000000000000UL) >> 56;
		value |= (x & 0x00FF000000000000UL) >> 40;
		value |= (x & 0x0000FF0000000000UL) >> 24;
		value |= (x & 0x000000FF00000000UL) >> 8;
		value |= (x & 0x00000000FF000000UL) << 8;
		value |= (x & 0x0000000000FF0000UL) << 24;
		value |= (x & 0x000000000000FF00UL) << 40;
		value |= (x & 0x00000000000000FFUL) << 56;
	} else {
		value = x;
	}

	return value;
}

extern int HIBUF_U64_TO_BYTES(uint64_t value, unsigned char *bytes, int size)
{
	int n;
	size_t nbytes;
	/*conver to network order(big endian)*/
	union {
		uint64_t u64;
		unsigned char byte[HIBUF_TYPE_U64_SIZE];
	}u;

	u.u64 = HIBUF_HTON64(value);
	for (n=0; n<HIBUF_TYPE_U64_SIZE; ++n) {
		/*
 			filter high bit zero(network order)
			e.g. 1 = 0000 0000 ... 0001
		*/
		if ( u.byte[n] ) break;
	}

	//check byte buffer enough
	nbytes = HIBUF_TYPE_U64_SIZE - n;
	if ( nbytes > size ) return -ENOMEM;

	memcpy(bytes, &u.byte[n], nbytes);	
	return nbytes;	
}
extern uint64_t  HIBUF_BYTES_TO_U64(int width, unsigned char *bytes, int size)
{
	uint64_t val = 0;
	/*
 		bytes is network order, may drop high bit
		e.g. 1 = 0000 0000 ... 00001
	*/
	if ( width < size ) {
		bytes += (width - size);//drop high bit
		size = width;
	}

	//conver to host order(little endian)
	while ( size ) {
		//move low -> high
		val <<= 8;
		val |= *(unsigned char *)bytes;
		size--;
		bytes++;
	}
	return val;
}

extern int64_t HIBUF_BYTES_TO_S64(int width, unsigned char *bytes, int size)
{
	union {	
		int64_t u64;
		unsigned char byte[HIBUF_TYPE_U64_SIZE];
	}u;

	//network order handle
	//[8-len] = get fillinfo pos of array
	memcpy(&u.byte[HIBUF_TYPE_U64_SIZE - size], bytes, size);

	//fill into msb(most signed bit)
	memset(u.byte, ((u.byte[HIBUF_TYPE_U64_SIZE - size]&0x80)?0xFF:0x00), 
		HIBUF_TYPE_U64_SIZE-size);

	return HIBUF_NTOH64(u.u64);
}

extern size_t hibuf_byte_length(const hibuf_meta_t * meta, void *object)
{
	if ( NULL == meta || NULL == object ) {
		return 0;
	}

	return HIBUF_OPT_OBJECT.bytesize(meta, NULL, object);
}
extern int    hibuf_byte_encode(const hibuf_meta_t * meta, void *object, char **bytes)
{
	int ret = 0, size = 0;
	if ( NULL == meta || NULL == object 
		|| NULL == bytes ) {
		return -EINVAL;	
	}
	
	size = hibuf_byte_length(meta, object);
	if ( 0 == size ) {
		return 0;
	}

	char * buffer = calloc(1, size);		
	if ( NULL == buffer ) {
		return -ENOMEM;
	}

	ret = hibuf_encode(meta, object, buffer, size);
	//handler error or encode length zero
	if ( ret <= 0 ) {
		free(buffer);
		buffer = NULL;
		return ret;
	} 
	*bytes = buffer;
	return ret;
}
extern void   hibuf_byte_free(char *bytes)
{
	if ( bytes ) free(bytes);
	return;
}

extern int    hibuf_encode(const hibuf_meta_t * meta, void *object, char *bytes, size_t size)
{
	if ( NULL == meta || NULL == object 
		|| NULL == bytes || size < 0 ) {
		return -EINVAL;
	}
	if ( 0 == size ) {
		return 0;
	}
	return HIBUF_OPT_OBJECT.obj2bin(meta, NULL, object, bytes, size);
}

extern int    hibuf_decode(const hibuf_meta_t * meta, char *bytes, size_t size, void **object)
{
	int ret = -ENOENT;
	if ( NULL == meta || NULL == object 
		|| NULL == bytes || size < 0 )  {
		return -EINVAL;
	}
	
	char *elem = calloc(1, meta->size);
	if ( NULL == elem ) {
		return -ENOMEM;
	}

	ret =  HIBUF_OPT_OBJECT.ctor(meta, NULL, elem);
	if ( 0 != ret ) {
		HIBUF_OPT_OBJECT.dtor(meta, NULL, elem);
		free(elem);
		return ret;	
	}

	//size zero, return memory space
	if ( 0 == size ) {
		*object = elem;
		return 0;
	}

	ret = HIBUF_OPT_OBJECT.bin2obj(meta, NULL, elem, bytes, size);
	if ( 0 != ret ) {
		HIBUF_OPT_OBJECT.dtor(meta, NULL, elem);
		free(elem);
		return ret;
	}
	*object = elem;
	return 0;	
}

extern void   hibuf_free(const hibuf_meta_t * meta, void *object)
{
	int ret = 0;
	if ( NULL == meta || NULL == object ) {
		return;
	}
	if ( 0 == HIBUF_OPT_OBJECT.dtor(meta, NULL, object) ) {
		free(object);
	}
	return;
}

extern const  hibuf_field_t * hibuf_find_field(const hibuf_meta_t * meta, const int id)
{
	int n;
	const hibuf_field_t * field;
	HIBUF_FIELD_FOREACH(meta, n, field) {
		if ( id == field->id ) {
			return field;
		}
	}
	return NULL;
}
