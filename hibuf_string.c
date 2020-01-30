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

static int  __HIBUF_OPT_STRING_check(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                       const char *N, const char *F, const char *M, const long L)
{
	HIBUF_ASSERT( prop->size > 0 );
	return 0;
}

static int  __HIBUF_OPT_STRING_ctor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;
}
static int __HIBUF_OPT_STRING_dtor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void * object)
{
	return 0;
}
static int  __HIBUF_OPT_STRING_comp(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * c1, void * c2)
{
	return strncmp(c1, c2, prop->size);
}
static int  __HIBUF_OPT_STRING_copy(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * src, void * dst)
{
	strncpy(src, dst, prop->size);
	return 0;
}
static int  __HIBUF_OPT_STRING_obj2bin(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	int len = strnlen(object, prop->size);

	if ( 0 == len ) return 0;
	if ( len > size ) return -ENOSPC;

	memcpy(buffer, object, len);			
	return len;
}

static int  __HIBUF_OPT_STRING_bin2obj(const hibuf_meta_t *meta, const hibuf_field_t *prop,
                        void * object, char * buffer, size_t size)
{
	int len = (size > prop->size) ? prop->size : size;//get min size to truncase

	memcpy(object, buffer, len);
	if ( len < prop->size ) {
		memset(object + len, 0x00, prop->size - len);//for left bytes, append to zero
	}
	return 0;	
}
static size_t __HIBUF_OPT_STRING_bytesize(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
			void *object)
{
	return strnlen(object, prop->size);
}

const hibuf_opt_t HIBUF_OPT_STRING = {
	.type    = HIBUF_TYPE_STRING,
	.name    = "STRING",
	.check   = __HIBUF_OPT_STRING_check,
	.ctor    = __HIBUF_OPT_STRING_ctor,
	.dtor    = __HIBUF_OPT_STRING_dtor,
	.comp    = __HIBUF_OPT_STRING_comp,
	.copy    = __HIBUF_OPT_STRING_copy,
	.obj2bin = __HIBUF_OPT_STRING_obj2bin,
	.bin2obj = __HIBUF_OPT_STRING_bin2obj,
	.bytesize = __HIBUF_OPT_STRING_bytesize,
};
