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

static int  __HIBUF_OPT_S16_check(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                       const char *N, const char *F, const char *M, const long L)
{
	HIBUF_ASSERT( HIBUF_TYPE_S16_SIZE == prop->size );
	return 0;
}

static int  __HIBUF_OPT_S16_ctor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;	
}
static int __HIBUF_OPT_S16_dtor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;
}
static int  __HIBUF_OPT_S16_comp(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * c1, void * c2)
{
	return HIBUF_NUMBER_MAX(*((int16_t *)c1), *((int16_t *)c2));
}
static int  __HIBUF_OPT_S16_copy(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * src, void * dst)
{
	*(int16_t *)dst = *(int16_t *)src;
	return 0;
}
static int  __HIBUF_OPT_S16_obj2bin(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	int16_t value = *(int16_t *)object;
	if (  size < HIBUF_TYPE_S16_SIZE ) return -ENOSPC;

	buffer[0] = (unsigned char)(value >> 8);
	buffer[1] = (unsigned char)value;
	return HIBUF_TYPE_S16_SIZE;
}

static int  __HIBUF_OPT_S16_bin2obj(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	int16_t * val = (int16_t *)object;
	*val = HIBUF_BYTES_TO_S64(HIBUF_TYPE_S16_SIZE, buffer, size);
	return 0;
}
static size_t __HIBUF_OPT_S16_bytesize(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
			void *object)
{
	return HIBUF_TYPE_S16_SIZE;
}

const hibuf_opt_t HIBUF_OPT_S16 = {
	.type    = HIBUF_TYPE_S16,
	.name    = "S16",
	.check   = __HIBUF_OPT_S16_check,
	.ctor    = __HIBUF_OPT_S16_ctor,
	.dtor    = __HIBUF_OPT_S16_dtor,
	.comp    = __HIBUF_OPT_S16_comp,
	.copy    = __HIBUF_OPT_S16_copy,
	.obj2bin = __HIBUF_OPT_S16_obj2bin,
	.bin2obj = __HIBUF_OPT_S16_bin2obj,
	.bytesize = __HIBUF_OPT_S16_bytesize,
};
