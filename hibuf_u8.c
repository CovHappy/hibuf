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

static int  __HIBUF_OPT_U8_check(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        const char *N, const char *F, const char *M, const long L)
{
	HIBUF_ASSERT( HIBUF_TYPE_U8_SIZE == prop->size );
	return 0;
}

static int  __HIBUF_OPT_U8_ctor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;	
}
static int __HIBUF_OPT_U8_dtor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;
}
static int  __HIBUF_OPT_U8_comp(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * c1, void * c2)
{
	return HIBUF_NUMBER_MAX(*((uint8_t *)c1), *((uint8_t *)c2));
}
static int  __HIBUF_OPT_U8_copy(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * src, void * dst)
{
	*(uint8_t *)dst = *(uint8_t *)src;
	return 0;
}
static int  __HIBUF_OPT_U8_obj2bin(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	uint8_t data = *(uint8_t *)object;
	
	//buffer size not enough
	if ( size < HIBUF_TYPE_U8_SIZE ) return -ENOSPC;

	//ignore zero value
	if ( 0 == data ) return 0;	

	buffer[0] = data;
	return HIBUF_TYPE_U8_SIZE;
}

static int  __HIBUF_OPT_U8_bin2obj(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	//get low byte => (buffer + size - 1) => bigendian[0 hi][1 low]
	*((uint8_t *)object) = (uint8_t) *(buffer + size - 1);
	return 0;	
}
static size_t __HIBUF_OPT_U8_bytesize(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
			void *object)
{
	if ( 0 == (*(uint8_t *)object) ) return 0;

	return HIBUF_TYPE_U8_SIZE;
}

const hibuf_opt_t HIBUF_OPT_U8 = {
	.type    = HIBUF_TYPE_U8,
	.name    = "U8",
	.check   = __HIBUF_OPT_U8_check,
	.ctor    = __HIBUF_OPT_U8_ctor,
	.dtor    = __HIBUF_OPT_U8_dtor,
	.comp    = __HIBUF_OPT_U8_comp,
	.copy    = __HIBUF_OPT_U8_copy,
	.obj2bin = __HIBUF_OPT_U8_obj2bin,
	.bin2obj = __HIBUF_OPT_U8_bin2obj,
	.bytesize = __HIBUF_OPT_U8_bytesize,
};
