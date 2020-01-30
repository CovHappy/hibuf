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

static int  __HIBUF_OPT_U64_check(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                       const char *N, const char *F, const char *M, const long L)
{
	HIBUF_ASSERT( HIBUF_TYPE_U64_SIZE == prop->size );
	return 0;
}

static int  __HIBUF_OPT_U64_ctor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;	
}
static int __HIBUF_OPT_U64_dtor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;
}
static int  __HIBUF_OPT_U64_comp(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * c1, void * c2)
{
	return HIBUF_NUMBER_MAX(*((uint64_t *)c1), *((uint64_t *)c2));
}
static int  __HIBUF_OPT_U64_copy(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * src, void * dst)
{
	*(uint64_t *)dst = *(uint64_t *)src;
	return 0;
}
static int  __HIBUF_OPT_U64_obj2bin(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	return HIBUF_U64_TO_BYTES(*(uint64_t *)object, buffer, size);
}

static int  __HIBUF_OPT_U64_bin2obj(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	*((uint64_t *)object) = (uint64_t)HIBUF_BYTES_TO_U64(HIBUF_TYPE_U64_SIZE, buffer, size);
	return 0;	
}
static size_t __HIBUF_OPT_U64_bytesize(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
			void *object)
{
	char buffer[16]	 = {0};
	return HIBUF_U64_TO_BYTES(*(uint64_t *)object, buffer, 16);
}

const hibuf_opt_t HIBUF_OPT_U64 = {
	.type    = HIBUF_TYPE_U64,
	.name    = "U64",
	.check   = __HIBUF_OPT_U64_check,
	.ctor    = __HIBUF_OPT_U64_ctor,
	.dtor    = __HIBUF_OPT_U64_dtor,
	.comp    = __HIBUF_OPT_U64_comp,
	.copy    = __HIBUF_OPT_U64_copy,
	.obj2bin = __HIBUF_OPT_U64_obj2bin,
	.bin2obj = __HIBUF_OPT_U64_bin2obj,
	.bytesize = __HIBUF_OPT_U64_bytesize,
};
