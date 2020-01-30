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

static int  __HIBUF_OPT_FLOAT_check(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                       const char *N, const char *F, const char *M, const long L)
{
	HIBUF_ASSERT( HIBUF_TYPE_FLOAT_SIZE == prop->size );
	return 0;
}

static int  __HIBUF_OPT_FLOAT_ctor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;	
}
static int __HIBUF_OPT_FLOAT_dtor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;
}
static int  __HIBUF_OPT_FLOAT_comp(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * c1, void * c2)
{
	return HIBUF_NUMBER_MAX(*((float *)c1), *((float *)c2));
}
static int  __HIBUF_OPT_FLOAT_copy(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * src, void * dst)
{
	*(float *)dst = *(float *)src;
	return 0;
}
static int  __HIBUF_OPT_FLOAT_obj2bin(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	union {
		char byte[4];
		uint32_t u32;
	}u;
	if ( size < HIBUF_TYPE_FLOAT_SIZE ) return -ENOSPC;

	u.u32 = *(uint32_t*)object;
	u.u32 = HIBUF_HTON32(u.u32);
	memcpy(buffer, u.byte, HIBUF_TYPE_FLOAT_SIZE);

	return HIBUF_TYPE_FLOAT_SIZE;
}

static int  __HIBUF_OPT_FLOAT_bin2obj(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	union {
		char byte[8];	
		uint32_t u32;
		uint64_t u64;
		float    f;
		double   d;
	}u;

	if ( size != HIBUF_TYPE_FLOAT_SIZE 
		&& size != HIBUF_TYPE_DOUBLE_SIZE ) {
		return -ENOSPC;
	}

	memcpy(u.byte, buffer, size);	
	if ( HIBUF_TYPE_FLOAT_SIZE == size ) {
		u.u32 = HIBUF_NTOH32(u.u32);
	} else {
		u.u64 = HIBUF_NTOH64(u.u64);
	}
	*(float *)object = u.f;
	return 0;	
}
static size_t __HIBUF_OPT_FLOAT_bytesize(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
			void *object)
{
	return HIBUF_TYPE_FLOAT_SIZE;
}

const hibuf_opt_t HIBUF_OPT_FLOAT = {
	.type    = HIBUF_TYPE_FLOAT,
	.name    = "FLOAT",
	.check   = __HIBUF_OPT_FLOAT_check,
	.ctor    = __HIBUF_OPT_FLOAT_ctor,
	.dtor    = __HIBUF_OPT_FLOAT_dtor,
	.comp    = __HIBUF_OPT_FLOAT_comp,
	.copy    = __HIBUF_OPT_FLOAT_copy,
	.obj2bin = __HIBUF_OPT_FLOAT_obj2bin,
	.bin2obj = __HIBUF_OPT_FLOAT_bin2obj,
	.bytesize = __HIBUF_OPT_FLOAT_bytesize,
};
