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

static int  __HIBUF_OPT_DOUBLE_check(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                       const char *N, const char *F, const char *M, const long L)
{
	HIBUF_ASSERT( HIBUF_TYPE_DOUBLE_SIZE == prop->size );
	return 0;
}

static int  __HIBUF_OPT_DOUBLE_ctor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;	
}
static int __HIBUF_OPT_DOUBLE_dtor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;
}
static int  __HIBUF_OPT_DOUBLE_comp(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * c1, void * c2)
{
	return HIBUF_NUMBER_MAX(*((double *)c1), *((double *)c2));
}
static int  __HIBUF_OPT_DOUBLE_copy(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * src, void * dst)
{
	*(double *)dst = *(double *)src;
	return 0;
}
static int  __HIBUF_OPT_DOUBLE_obj2bin(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	union {
		char byte[8];
		uint64_t u64;
	}u;

	if ( size < HIBUF_TYPE_DOUBLE_SIZE ) return -ENOSPC;

	u.u64 = *(uint64_t*)object;
	u.u64 = HIBUF_HTON64(u.u64);
	memcpy(buffer, u.byte, HIBUF_TYPE_DOUBLE_SIZE);

	return HIBUF_TYPE_DOUBLE_SIZE;
}

static int  __HIBUF_OPT_DOUBLE_bin2obj(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	union {
		char byte[8];
		float f;
		double d;
		uint64_t u64;
		uint64_t u32;
	}u;

	if ( size != HIBUF_TYPE_FLOAT_SIZE && size != HIBUF_TYPE_DOUBLE_SIZE ) {
		return -ENOSPC;
	}

	memcpy(u.byte, buffer, size);	
	if ( size == HIBUF_TYPE_FLOAT_SIZE ) {
		u.u32 = HIBUF_HTON32(u.u32);
	} else {
		u.u64 = HIBUF_HTON64(u.u64);
	}
	*(double *)object = u.d;
	return 0;	
}
static size_t __HIBUF_OPT_DOUBLE_bytesize(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
			void *object)
{
	return HIBUF_TYPE_DOUBLE_SIZE;
}

const hibuf_opt_t HIBUF_OPT_DOUBLE = {
	.type    = HIBUF_TYPE_DOUBLE,
	.name    = "DOUBLE",
	.check   = __HIBUF_OPT_DOUBLE_check,
	.ctor    = __HIBUF_OPT_DOUBLE_ctor,
	.dtor    = __HIBUF_OPT_DOUBLE_dtor,
	.comp    = __HIBUF_OPT_DOUBLE_comp,
	.copy    = __HIBUF_OPT_DOUBLE_copy,
	.obj2bin = __HIBUF_OPT_DOUBLE_obj2bin,
	.bin2obj = __HIBUF_OPT_DOUBLE_bin2obj,
	.bytesize = __HIBUF_OPT_DOUBLE_bytesize,
};
