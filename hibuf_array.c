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

static int __HIBUF_OPT_ARRAY_check(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
	const char *N, const char *F, const char *M, const long L)
{
	const hibuf_meta_t *objmeta  = prop->ref;

	HIBUF_ASSERT( NULL != objmeta );
	HIBUF_ASSERT( sizeof(hibuf_array_t) == prop->size);
	HIBUF_ASSERT( objmeta->count > 0 );//num of fields
	HIBUF_ASSERT( NULL != objmeta->fields );//fields pointer
	
	return 0;
}
static int __HIBUF_OPT_ARRAY_ctor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void *object)
{
	hibuf_array_t *array = object;
	array->data = NULL;	
	array->count = 0;
	return 0;
}
static int __HIBUF_OPT_ARRAY_dtor(const hibuf_meta_t *meta, const hibuf_field_t *prop, void *object)
{
	int n = 0, ret = 0;
	void *elem = NULL;
	const hibuf_meta_t *objmeta = prop->ref;
	hibuf_array_t *array = object;
	
	if ( array->count ) {
		HIBUF_ARRAY_FOREACH(objmeta, n, array, elem) {
			//field == prop == array field, but meta is object meta
			ret = HIBUF_OPT_OBJECT.dtor(objmeta, NULL, elem);
			if ( 0 != ret ) break;
		}
		free(array->data);
	}
	array->data = NULL;
	array->count = 0;
	return ret;
}
static int __HIBUF_OPT_ARRAY_comp(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
			void *c1, void *c2)
{
	int n = 0, ret = 0;
	const hibuf_meta_t *objmeta = prop->ref;
	hibuf_array_t *a1 = c1;
	hibuf_array_t *a2 = c2;
	void * o1 = NULL;
	void * o2 = NULL;
	
	if ( a1->count > a2->count ) {
		return 1;	
	} 
	if ( a1->count < a2->count ) {
		return -1;
	}

	//count same
	for(n=0, o1=a1->data, o2=a2->data; n<a1->count; 
		++n, o1+=objmeta->size, o2+=objmeta->size) {
		ret = HIBUF_OPT_OBJECT.comp(objmeta, NULL, o1, o2);
		if ( 0 != ret ) break;
	}
	return ret;
}
static int __HIBUF_OPT_ARRAY_copy(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
		void *src, void *dst)
{
	int n = 0, ret = 0;
	const hibuf_meta_t *objmeta = prop->ref;
	hibuf_array_t *a1 = src;
	hibuf_array_t *a2 = dst;
	void * o1 = NULL;
	void * o2 = NULL;

	//init dst data=NULL and count=0
	__HIBUF_OPT_ARRAY_dtor(meta, prop, dst);

	//check src count valid
	if ( a1->count <= 0 ) {
		return ret;
	}

	//for dst malloc space
	a2->data = calloc(a1->count, objmeta->size);
	if ( NULL == a2->data ) {
		return -ENOMEM;
	}

	for(n=0, o1=a1->data, o2=a2->data; n<a1->count; 
		++n, o1+=objmeta->size, o2+=objmeta->size) {
		ret = HIBUF_OPT_OBJECT.copy(objmeta, NULL, o1, o2);
		if ( 0 != ret ) break;
	}
	return ret;
}

static int __HIBUF_OPT_ARRAY_obj2bin(const hibuf_meta_t *meta, const hibuf_field_t *prop,
		void *object, char *buffer, size_t size)
{
	int n = 0, ret = 0, nbytes = 0;
	const hibuf_meta_t *objmeta = prop->ref;
	hibuf_array_t *array = object;
	void *elem = NULL;

	//head format: LV1,LV2,LV3
	HIBUF_ARRAY_FOREACH(objmeta, n, array, elem) {
		//check head size enough
		if ( nbytes + HIBUF_HEAD_LEN_SIZE >= size ) {
			return -ENOSPC;
		}

		//VALUE
		ret = HIBUF_OPT_OBJECT.obj2bin(objmeta, NULL, elem, 
			buffer + nbytes + HIBUF_HEAD_LEN_SIZE, 
			size - nbytes - HIBUF_HEAD_LEN_SIZE);

		//handle error
		if ( ret < 0 ) return ret;
		//handle carray
		if ( ret > 0xFFFFU ) return -ret;

		//LEN 
		buffer[nbytes]     =  (unsigned char)ret>>8;
		buffer[nbytes + 1] |= (unsigned char)ret;
		
		//update nbytes
		nbytes += ret + HIBUF_HEAD_LEN_SIZE;
	}
	return nbytes;
}

static int __HIBUF_OPT_ARRAY_bin2obj(const hibuf_meta_t *meta, const hibuf_field_t *prop, 
		void *object, char * buffer, size_t size)
{
	int n = 0, ret = 0, nbytes = 0;
	uint16_t len = 0;
	const hibuf_meta_t *objmeta = prop->ref;
	hibuf_array_t *array = object;
	hibuf_field_t * field = NULL;
	void * elem = NULL;

	//calc all element count, size = TOTAL {[LV1, V1={TLV}], [LV2, V2={TLV}], [LV3, V3={TLV}], }
	while ( nbytes + HIBUF_HEAD_LEN_SIZE < size ) {//check head valid
		//get current elem size, include [TLV]
		len =  buffer[nbytes]<<8;
		len |= buffer[nbytes + 1];

		//check all elem size over total size
		if ( (nbytes + len + HIBUF_HEAD_LEN_SIZE) > size ) {
			break;
		}
		//update nbytes
		nbytes += (len + HIBUF_HEAD_LEN_SIZE);
		n++;
	}

	if ( 0 == n ) return ret;

	__HIBUF_OPT_ARRAY_dtor(meta, prop, object);
	elem = array->data = calloc(n, objmeta->size);	
	if ( NULL == elem ) {
		return -ENOMEM;	
	}
	array->count = n;

	nbytes = 0;
	//format: {[LV1,V1={TLV}], LV2, V2={TLV}, LV3, V3={TLV}]
	while ( nbytes + HIBUF_HEAD_LEN_SIZE < size ) {
		//get LEN from head
		len = buffer[nbytes]<<8;
		len |= buffer[nbytes + 1];

		//check size enough
		if ( nbytes + len + HIBUF_HEAD_LEN_SIZE > size ) {
			break;
		}
		//get current elem position = V
		nbytes += HIBUF_HEAD_LEN_SIZE;

		//elem[TLV]
		HIBUF_OPT_OBJECT.bin2obj(objmeta, NULL, elem, 
			buffer + nbytes, len);

		//next TV position = LV
		nbytes += len;
		//get attribute for object
		elem += objmeta->size;
	}

	return ret;
}

static size_t __HIBUF_OPT_ARRAY_bytesize(const hibuf_meta_t *meta, const hibuf_field_t *prop, void *object)
{
	int n = 0, nbytes = 0, ret = 0;
	const hibuf_meta_t *objmeta = prop->ref;
	hibuf_array_t *array = object;
	void * elem = NULL;
	
	HIBUF_ARRAY_FOREACH(objmeta, n, array, elem) {
		ret = HIBUF_OPT_OBJECT.bytesize(objmeta, NULL, elem);
		nbytes += ret + HIBUF_HEAD_LEN_SIZE;	
	}
	return nbytes;
}

const hibuf_opt_t HIBUF_OPT_ARRAY = {
	.type    = HIBUF_TYPE_ARRAY,
	.name    = "ARRAY",
	.check   = __HIBUF_OPT_ARRAY_check,
	.ctor    = __HIBUF_OPT_ARRAY_ctor,
	.dtor    = __HIBUF_OPT_ARRAY_dtor,
	.comp    = __HIBUF_OPT_ARRAY_comp,
	.copy    = __HIBUF_OPT_ARRAY_copy,
	.obj2bin = __HIBUF_OPT_ARRAY_obj2bin,
	.bin2obj = __HIBUF_OPT_ARRAY_bin2obj,
	.bytesize = __HIBUF_OPT_ARRAY_bytesize,
};
