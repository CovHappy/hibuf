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

static int  __HIBUF_OPT_Z64_check(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                       const char *N, const char *F, const char *M, const long L)
{
	HIBUF_ASSERT( HIBUF_TYPE_Z64_SIZE == prop->size );
	return 0;
}

static int  __HIBUF_OPT_Z64_ctor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;	
}
static int __HIBUF_OPT_Z64_dtor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;
}
static int  __HIBUF_OPT_Z64_comp(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * c1, void * c2)
{
	return HIBUF_NUMBER_MAX(*((int64_t *)c1), *((int64_t *)c2));
}
static int  __HIBUF_OPT_Z64_copy(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * src, void * dst)
{
	*(int64_t *)dst = *(int64_t *)src;
	return 0;
}
static int  __HIBUF_OPT_Z64_obj2bin(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	//first, zigzag encode
	uint64_t val = ZZ_S64ENCODE(*(int64_t *)object);

	//second, convert to bytes
	return HIBUF_U64_TO_BYTES(val, buffer, size);
}

static int  __HIBUF_OPT_Z64_bin2obj(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	//first, convert to bytes
	uint64_t orig = HIBUF_BYTES_TO_U64(HIBUF_TYPE_Z64_SIZE, buffer, size);

	//second, zigzag decode
	int64_t val = ZZ_S64DECODE(orig);

	*((int64_t *)object) = (int64_t)val;
	return 0;	
}
static size_t __HIBUF_OPT_Z64_bytesize(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
			void *object)
{
	char buffer[16] = {0};

	//first, zigzag encode
	uint64_t val = ZZ_S64ENCODE((*(int64_t *)object));

	//second, convert to bytes
	return HIBUF_U64_TO_BYTES((uint64_t)val, buffer, 16);
}

const hibuf_opt_t HIBUF_OPT_Z64 = {
	.type    = HIBUF_TYPE_Z64,
	.name    = "ZIGZAG64",
	.check   = __HIBUF_OPT_Z64_check,
	.ctor    = __HIBUF_OPT_Z64_ctor,
	.dtor    = __HIBUF_OPT_Z64_dtor,
	.comp    = __HIBUF_OPT_Z64_comp,
	.copy    = __HIBUF_OPT_Z64_copy,
	.obj2bin = __HIBUF_OPT_Z64_obj2bin,
	.bin2obj = __HIBUF_OPT_Z64_bin2obj,
	.bytesize = __HIBUF_OPT_Z64_bytesize,
};
