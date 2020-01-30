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

static int  __HIBUF_OPT_S64_check(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                       const char *N, const char *F, const char *M, const long L)
{
	HIBUF_ASSERT( HIBUF_TYPE_S64_SIZE == prop->size );
	return 0;
}

static int  __HIBUF_OPT_S64_ctor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;	
}
static int __HIBUF_OPT_S64_dtor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;
}
static int  __HIBUF_OPT_S64_comp(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * c1, void * c2)
{
	return HIBUF_NUMBER_MAX(*((int64_t *)c1), *((int64_t *)c2));
}
static int  __HIBUF_OPT_S64_copy(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * src, void * dst)
{
	*(int64_t *)dst = *(int64_t *)src;
	return 0;
}
static int  __HIBUF_OPT_S64_obj2bin(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	int64_t val = *(int64_t*)object;
	if ( size < HIBUF_TYPE_S64_SIZE ) return -ENOSPC;

	buffer[0] = (unsigned char)(val>>56);
	buffer[1] = (unsigned char)(val>>48);
	buffer[2] = (unsigned char)(val>>40);
	buffer[3] = (unsigned char)(val>>32); 
	buffer[4] = (unsigned char)(val>>24); 
	buffer[5] = (unsigned char)(val>>16); 
	buffer[6] = (unsigned char)(val>>8); 
	buffer[7] = (unsigned char)val; 

	return HIBUF_TYPE_S64_SIZE;
}

static int  __HIBUF_OPT_S64_bin2obj(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	*((int64_t *)object) = (int64_t)HIBUF_BYTES_TO_S64(HIBUF_TYPE_S64_SIZE, buffer, size);
	return 0;	
}
static size_t __HIBUF_OPT_S64_bytesize(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
			void *object)
{
	return HIBUF_TYPE_S64_SIZE;
}

const hibuf_opt_t HIBUF_OPT_S64 = {
	.type    = HIBUF_TYPE_S64,
	.name    = "S64",
	.check   = __HIBUF_OPT_S64_check,
	.ctor    = __HIBUF_OPT_S64_ctor,
	.dtor    = __HIBUF_OPT_S64_dtor,
	.comp    = __HIBUF_OPT_S64_comp,
	.copy    = __HIBUF_OPT_S64_copy,
	.obj2bin = __HIBUF_OPT_S64_obj2bin,
	.bin2obj = __HIBUF_OPT_S64_bin2obj,
	.bytesize = __HIBUF_OPT_S64_bytesize,
};
