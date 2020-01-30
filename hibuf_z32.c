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

static int  __HIBUF_OPT_Z32_check(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                       const char *N, const char *F, const char *M, const long L)
{
	HIBUF_ASSERT( HIBUF_TYPE_Z32_SIZE == prop->size );
	return 0;
}

static int  __HIBUF_OPT_Z32_ctor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;	
}
static int __HIBUF_OPT_Z32_dtor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;
}
static int  __HIBUF_OPT_Z32_comp(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * c1, void * c2)
{
	return HIBUF_NUMBER_MAX(*((int32_t *)c1), *((int32_t *)c2));
}
static int  __HIBUF_OPT_Z32_copy(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * src, void * dst)
{
	*(int32_t *)dst = *(int32_t *)src;
	return 0;
}
static int  __HIBUF_OPT_Z32_obj2bin(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	//first, zigzag encode
	uint32_t val = ZZ_S32ENCODE((*(int32_t *)object));

	//second, convert to bytes
	return HIBUF_U64_TO_BYTES((uint64_t)val, buffer, size);		
}

static int  __HIBUF_OPT_Z32_bin2obj(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	//first, convert to u64
	uint32_t orig = (uint32_t)HIBUF_BYTES_TO_U64(HIBUF_TYPE_Z32_SIZE, buffer, size);

	//second, zigzag decode
	int32_t val = ZZ_S32DECODE(orig);

	*((int32_t *)object) = val;
	return 0;	
}
static size_t __HIBUF_OPT_Z32_bytesize(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
			void *object)
{
	char buffer[16]	 = {0};

	//first, zigzag encode
	uint32_t val = ZZ_S32ENCODE((*(int32_t *)object));

	return HIBUF_U64_TO_BYTES((uint64_t)val, buffer, 16);
}

const hibuf_opt_t HIBUF_OPT_Z32 = {
	.type    = HIBUF_TYPE_Z32,
	.name    = "ZIGZAG32",
	.check   = __HIBUF_OPT_Z32_check,
	.ctor    = __HIBUF_OPT_Z32_ctor,
	.dtor    = __HIBUF_OPT_Z32_dtor,
	.comp    = __HIBUF_OPT_Z32_comp,
	.copy    = __HIBUF_OPT_Z32_copy,
	.obj2bin = __HIBUF_OPT_Z32_obj2bin,
	.bin2obj = __HIBUF_OPT_Z32_bin2obj,
	.bytesize = __HIBUF_OPT_Z32_bytesize,
};
