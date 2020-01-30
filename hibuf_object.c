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

static int __HIBUF_OPT_OBJECT_check(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
	const char *N, const char *F, const char *M, const long L)
{
	HIBUF_ASSERT( NULL != meta );
	HIBUF_ASSERT( NULL != meta->fields );
	HIBUF_ASSERT( 0 != meta->size );
	HIBUF_ASSERT( 0 != meta->count );
	return 0;
}
static int __HIBUF_OPT_OBJECT_ctor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void *object)
{
	int n = 0, ret = 0;
	const hibuf_field_t *field = NULL;

	HIBUF_FIELD_FOREACH(meta, n, field) {
		ret = field->opt->ctor(meta, 
			field, (object + field->offset));
		if ( 0 != ret ) break;
	}
	return ret;
}
static int __HIBUF_OPT_OBJECT_dtor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void *object)
{
	int n = 0, ret = 0;
	const hibuf_field_t *field = NULL;
	
	HIBUF_FIELD_FOREACH(meta, n, field) {
		ret = field->opt->dtor(meta, 
				field, (object + field->offset));
		if ( 0 != ret ) break;
	}	
	return ret;
}
static int __HIBUF_OPT_OBJECT_comp(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
			void *c1, void *c2)
{
	int n = 0, ret = 0;
	const hibuf_field_t *field = NULL;
	
	if ( !c1 && !c2 ) {
		return 0;
	}
	if ( !c1 && c2 ) {
		return 1;
	}
	if (  c1 && !c2 ) {
		return -1;
	}
	HIBUF_FIELD_FOREACH(meta, n, field) {
		ret = field->opt->comp(meta, 
			field, (c1+field->offset), (c2+field->offset));
		//check not equals
		if ( 0 != ret ) break;
	}

	return ret;
}
static int __HIBUF_OPT_OBJECT_copy(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
		void *src, void *dst)
{
	int n = 0, ret = 0;
	const hibuf_field_t *field = NULL;

	HIBUF_FIELD_FOREACH(meta, n, field) {
		ret = field->opt->copy(meta, 
				field, (src+field->offset), (dst+field->offset));
		if ( 0 != ret ) break;
	}		
	
	return 0;
}
static int __HIBUF_OPT_OBJECT_obj2bin(const hibuf_meta_t *meta, const hibuf_field_t *prop,
		void *object, char *buffer, size_t size)
{
	int n = 0, ret = 0, nbytes = 0;
	char *   val = 0;
	const hibuf_field_t * field = NULL;
	uint16_t * head = NULL;

	HIBUF_FIELD_FOREACH(meta, n, field) {
		
		//space not enough
		if ( nbytes + HIBUF_HEAD_SIZE >= size ) {
			return -ENOSPC;
		}

		//[V]first offset body position for buffer, return body length
		ret = field->opt->obj2bin(meta, 
			field, (object + field->offset), 
			buffer + nbytes + HIBUF_HEAD_SIZE, 
			size - nbytes - HIBUF_HEAD_SIZE);
		//handle error
		if ( ret < 0 ) {
			return ret;
		} else if ( 0 == ret ) {
			continue;
		}  

		//add head[TL]
		//T
		buffer[nbytes]     =  field->id >> 8;//HI
		buffer[nbytes + 1] =  field->id; //LO
	
		//L
		buffer[nbytes + 2] = ret >> 8;//HI
		buffer[nbytes + 3] = ret;//LO

		//update newest length
		nbytes += HIBUF_HEAD_SIZE + ret;
	}
	return nbytes;
}
static int __HIBUF_OPT_OBJECT_bin2obj(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
		void *object, char * buffer, size_t size)
{
	int n = 0, ret = 0, nbytes = 0;
	uint16_t tag = 0;
	uint16_t len = 0;
	char *   val = 0;
	const hibuf_field_t * field = NULL;

	//TLV format = T[2byte]L[2byte]V[*]
	//check head enough
	while ( (nbytes + HIBUF_HEAD_SIZE) < size ) {

		//get TAG, LEN info
		//T
		tag = buffer[nbytes]<<8;
		tag |= buffer[nbytes + 1];
		//L
		len = buffer[nbytes + 2]<<8;
		len |= buffer[nbytes + 3];
		
		val = buffer + nbytes + HIBUF_HEAD_SIZE;

		//check [head + body] enough
		if ( nbytes + HIBUF_HEAD_SIZE + len > size ) {
			return -EFAULT;//bad address	
		}

		//update bytes of data
		nbytes += (HIBUF_HEAD_SIZE + len);
		
		field = hibuf_find_field(meta, tag);
		if ( NULL == field ) continue;

		//val is buffer of next field
		ret = field->opt->bin2obj(meta, 
			field, object + field->offset, val, len);

		//handle error 
		if ( ret < 0 ) break;
	}
	return ret;
}
static size_t __HIBUF_OPT_OBJECT_bytesize(const hibuf_meta_t *meta, const hibuf_field_t *prop, void *object)
{
	int n = 0, nbytes = 0, ret = 0;
	const hibuf_field_t * field = NULL;	

	//format: TLV = HIBUF_HEAD_SIZE + BODY LEN
	HIBUF_FIELD_FOREACH(meta, n, field) {
		ret = field->opt->bytesize(meta, field, object + field->offset);
		nbytes += ret + HIBUF_HEAD_SIZE;
	}
	return nbytes;
}

const hibuf_opt_t HIBUF_OPT_OBJECT = {
	.type    = HIBUF_TYPE_OBJECT,
	.name    = "OBJECT",
	.check   = __HIBUF_OPT_OBJECT_check,
	.ctor    = __HIBUF_OPT_OBJECT_ctor,
	.dtor    = __HIBUF_OPT_OBJECT_dtor,
	.comp    = __HIBUF_OPT_OBJECT_comp,
	.copy    = __HIBUF_OPT_OBJECT_copy,
	.obj2bin = __HIBUF_OPT_OBJECT_obj2bin,
	.bin2obj = __HIBUF_OPT_OBJECT_bin2obj,
	.bytesize = __HIBUF_OPT_OBJECT_bytesize,
};
