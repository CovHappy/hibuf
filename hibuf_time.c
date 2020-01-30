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

static int  __HIBUF_OPT_TIME_check(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                       const char *N, const char *F, const char *M, const long L)
{
	HIBUF_ASSERT( HIBUF_TYPE_TIME_SIZE == prop->size );
	return 0;
}

static int  __HIBUF_OPT_TIME_ctor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;	
}
static int __HIBUF_OPT_TIME_dtor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;
}
static int  __HIBUF_OPT_TIME_comp(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * c1, void * c2)
{
	return HIBUF_NUMBER_MAX(*((int32_t *)c1), *((int32_t *)c2));
}
static int  __HIBUF_OPT_TIME_copy(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * src, void * dst)
{
	*(int32_t *)dst = *(int32_t *)src;
	return 0;
}
static int  __HIBUF_OPT_TIME_obj2bin(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	const hibuf_opt_t * opt = 
		(HIBUF_TYPE_TIME_SIZE == HIBUF_TYPE_S32_SIZE) 
			? &HIBUF_OPT_S32 : &HIBUF_OPT_S64;
	return opt->obj2bin(meta, prop, object, buffer, size);
}

static int  __HIBUF_OPT_TIME_bin2obj(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	*(time_t *) object = (time_t) HIBUF_BYTES_TO_S64(HIBUF_TYPE_TIME_SIZE, buffer, size);
	return 0;	
}
static size_t __HIBUF_OPT_TIME_bytesize(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
			void *object)
{
	return HIBUF_TYPE_TIME_SIZE;
}

const hibuf_opt_t HIBUF_OPT_TIME = {
	.type    = HIBUF_TYPE_TIME,
	.name    = "TIME",
	.check   = __HIBUF_OPT_TIME_check,
	.ctor    = __HIBUF_OPT_TIME_ctor,
	.dtor    = __HIBUF_OPT_TIME_dtor,
	.comp    = __HIBUF_OPT_TIME_comp,
	.copy    = __HIBUF_OPT_TIME_copy,
	.obj2bin = __HIBUF_OPT_TIME_obj2bin,
	.bin2obj = __HIBUF_OPT_TIME_bin2obj,
	.bytesize = __HIBUF_OPT_TIME_bytesize,
};
