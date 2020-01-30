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

#ifndef __HIBUF_H__
#define __HIBUF_H__ 1

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>
#include <endian.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __hibuf_meta__ hibuf_meta_t;
typedef struct __hibuf_opt__ hibuf_opt_t;

/*
 * encode: 
 *         ZigZag(n)
 *            	(-11) = 11110101
 *              N>>7  = 11111111 (by most significant bit，MSB)
 *              N<<1  = 11101010
 *              ^     = 00010101

   decode:
 *         (ZigZag^-1)(n)
 *                 (-11) = 00010101
 *                 N>>1  = 00001010
 *                 N&1   = 00000001
 *                 -     = 11111111
 *                 ^     = 11110101
 */

#define ZZ_DECODE(N) ((N)>>1)^(-((N)&1))
//#define ZZ_DECODE(N) ((N)>>1)^(~((N)&1)+1)

#define ZZ_S32ENCODE(N) (((uint32_t)(N>>31))^(((uint32_t)(N))<<1))
#define ZZ_S32DECODE(N) ZZ_DECODE(((uint32_t)(N)))

#define ZZ_S64ENCODE(N) (((uint64_t)(N>>63))^(((uint64_t)(N))<<1))
#define ZZ_S64DECODE(N) ZZ_DECODE(((uint64_t)(N)))

#define HIBUF_NUMBER_MAX(A, B) ((A)==(B))?0:((A)>(B)?1:-1)

#define HIBUF_HEAD_TAG_SIZE 2
#define HIBUF_HEAD_LEN_SIZE 2
#define HIBUF_HEAD_SIZE (HIBUF_HEAD_TAG_SIZE + HIBUF_HEAD_LEN_SIZE)

#define HIBUF_TYPE_U8_SIZE  1
#define HIBUF_TYPE_U16_SIZE 2
#define HIBUF_TYPE_U32_SIZE 4
#define HIBUF_TYPE_U64_SIZE 8

#define HIBUF_TYPE_S8_SIZE  HIBUF_TYPE_U8_SIZE
#define HIBUF_TYPE_S16_SIZE HIBUF_TYPE_U16_SIZE
#define HIBUF_TYPE_S32_SIZE HIBUF_TYPE_U32_SIZE
#define HIBUF_TYPE_S64_SIZE HIBUF_TYPE_U64_SIZE

#define HIBUF_TYPE_Z32_SIZE HIBUF_TYPE_S32_SIZE
#define HIBUF_TYPE_Z64_SIZE HIBUF_TYPE_S64_SIZE

#define HIBUF_TYPE_TIME_SIZE sizeof(time_t)
#define HIBUF_TYPE_FLOAT_SIZE sizeof(float)
#define HIBUF_TYPE_DOUBLE_SIZE sizeof(double)

#if __BYTE_ORDER == __BIG_ENDIAN
#define HIBUF_BIG_ENDIAN 1
#else
#define HIBUF_BIG_ENDIAN 0
#endif

typedef enum {
	HIBUF_TYPE_UNKNOW,

	HIBUF_TYPE_U8,
	HIBUF_TYPE_U16,
	HIBUF_TYPE_U32,
	HIBUF_TYPE_U64,

	HIBUF_TYPE_S8,
	HIBUF_TYPE_S16,
	HIBUF_TYPE_S32,
	HIBUF_TYPE_S64,

	HIBUF_TYPE_Z32,
	HIBUF_TYPE_Z64,

	HIBUF_TYPE_FLOAT,
	HIBUF_TYPE_DOUBLE,

	HIBUF_TYPE_TIME,

	HIBUF_TYPE_STRING,
	HIBUF_TYPE_OBJECT,
	HIBUF_TYPE_ARRAY,
	HIBUF_TYPE_MAX,
#define HIBUF_TYPE_MAX HIBUF_TYPE_MAX
} hibuf_type_e;

#define HIBUF_ARRAY_FOREACH(M, N, A, V) \
	for (N=0, V=A->data; N<A->count; ++N,V+=M->size) \

#define HIBUF_FIELD_FOREACH(M, N, F) \
	for(N=0,F=M->fields; N<M->count; ++N,++F) \

#define HIBUF_ASSERT(EXP) do { \
	if ( !(EXP) ) { \
		printf("\033[0;0;1mHIBUF CHECK:\033[0m meta \033[40;31;1m`%s`\033[0m to assert \033[40;35;1m[%s]\033[0m at \033[40;34;1m(%s:%s:%lld)\033[0m\n", \
			N, #EXP, F, M, L); \
		abort(); \
	} \
}while(0)
	
#define HIBUF_ARRAY_HEAD(name, type) \
	struct name { \
		struct type * data;\
		int count;\
	}

typedef struct __hibuf_array__ {
	void *data;
	int count;
}hibuf_array_t;

typedef struct __hibuf_field__ {
	uint16_t       		id;     //identify
	const char *    	name;   //attribute name
	uint32_t             	offset; //attribute offset pointer
	long             	size;   //attribute size
	const hibuf_opt_t *  	opt;    //operator api
	const void *    	ref;    //array meta
}hibuf_field_t;

struct __hibuf_meta__ {
	int size; // -> struct xxx
	int count;
	const hibuf_field_t *fields;
};

struct __hibuf_opt__ {
	hibuf_type_e type;
	char *name;
	int  (*check)(const hibuf_meta_t *, const hibuf_field_t *, 
			const char *, const char *, const char *, const long);
	int  (*ctor)(const hibuf_meta_t *, const hibuf_field_t *, void *);
	int  (*dtor)(const hibuf_meta_t *, const hibuf_field_t *, void *);
	int  (*comp) (const hibuf_meta_t *, const hibuf_field_t *, void *, void *);
	int  (*copy) (const hibuf_meta_t *, const hibuf_field_t *, void *, void *);
	int  (*obj2bin)(const hibuf_meta_t *, const hibuf_field_t *,
			void *, char *, size_t);
	int  (*bin2obj)(const hibuf_meta_t *, const hibuf_field_t *,
			void *, char *, size_t);
	size_t (*bytesize)(const hibuf_meta_t *, const hibuf_field_t *, void *);
};

extern const hibuf_opt_t HIBUF_OPT_S8;
extern const hibuf_opt_t HIBUF_OPT_S16;
extern const hibuf_opt_t HIBUF_OPT_S32;
extern const hibuf_opt_t HIBUF_OPT_S64;
extern const hibuf_opt_t HIBUF_OPT_U8;
extern const hibuf_opt_t HIBUF_OPT_U16;
extern const hibuf_opt_t HIBUF_OPT_U32;
extern const hibuf_opt_t HIBUF_OPT_U64;
extern const hibuf_opt_t HIBUF_OPT_Z32;
extern const hibuf_opt_t HIBUF_OPT_Z64;
extern const hibuf_opt_t HIBUF_OPT_FLOAT;
extern const hibuf_opt_t HIBUF_OPT_DOUBLE;
extern const hibuf_opt_t HIBUF_OPT_TIME;
extern const hibuf_opt_t HIBUF_OPT_STRING;
extern const hibuf_opt_t HIBUF_OPT_ARRAY;
extern const hibuf_opt_t HIBUF_OPT_OBJECT;

#define HIBUF_FIELD(OP,I,N,O) {I, #N, offsetof(typeof(*______base), N), sizeof(______base->N), &HIBUF_OPT_##OP, O}

#define HIBUF_FIELD_U8(I,N)  	  HIBUF_FIELD(U8,I,N,NULL)
#define HIBUF_FIELD_U16(I,N) 	  HIBUF_FIELD(U16,I,N,NULL)
#define HIBUF_FIELD_U32(I,N) 	  HIBUF_FIELD(U32,I,N,NULL)
#define HIBUF_FIELD_U64(I,N) 	  HIBUF_FIELD(U64,I,N,NULL)
#define HIBUF_FIELD_S8(I,N)  	  HIBUF_FIELD(S8,I,N,NULL)
#define HIBUF_FIELD_S16(I,N)  	  HIBUF_FIELD(S16,I,N,NULL)
#define HIBUF_FIELD_S32(I,N)  	  HIBUF_FIELD(S32,I,N,NULL)
#define HIBUF_FIELD_S64(I,N)  	  HIBUF_FIELD(S64,I,N,NULL)
#define HIBUF_FIELD_Z32(I,N)  	  HIBUF_FIELD(Z32,I,N,NULL)
#define HIBUF_FIELD_Z64(I,N)  	  HIBUF_FIELD(Z64,I,N,NULL)
#define HIBUF_FIELD_TIME(I,N)  	  HIBUF_FIELD(TIME,I,N,NULL)
#define HIBUF_FIELD_FLOAT(I,N)    HIBUF_FIELD(FLOAT,I,N,NULL)
#define HIBUF_FIELD_DOUBLE(I,N)   HIBUF_FIELD(DOUBLE,I,N,NULL)
#define HIBUF_FIELD_STRING(I,N)   HIBUF_FIELD(STRING,I,N,NULL)
#define HIBUF_FIELD_ARRAY(I,N,O)  HIBUF_FIELD(ARRAY,I,N,O) 
#define HIBUF_FIELD_OBJECT(I,N,O) HIBUF_FIELD(OBJECT,I,N,O)

#define HIBUF_MAPPING(META,TYPE,FIELDS...) do{ \
	TYPE * ______base = NULL; \
	static const hibuf_field_t  __HIBUF_MAPPING_hibuf_field_t_##META[] = { FIELDS }; \
	META.fields = __HIBUF_MAPPING_hibuf_field_t_##META; \
	META.count  = sizeof(__HIBUF_MAPPING_hibuf_field_t_##META)/sizeof(hibuf_field_t); \
	META.size   = sizeof(TYPE); \
	HIBUF_CHECK(&META, #META, __FILE__, __func__, __LINE__); \
} while(0)

extern uint16_t HIBUF_HTON16(uint16_t net);
#define HIBUF_NTOH16(host) HIBUF_HTON16(host)
extern uint32_t HIBUF_HTON32(uint32_t net);
#define HIBUF_NTOH32(host) HIBUF_HTON32(host)
extern uint64_t HIBUF_HTON64(uint64_t net);
#define HIBUF_NTOH64(host) HIBUF_HTON64(host)

extern int HIBUF_U64_TO_BYTES(uint64_t value,  unsigned char *bytes, int size);
extern uint64_t  HIBUF_BYTES_TO_U64(int width, unsigned char *bytes, int size);
extern int64_t   HIBUF_BYTES_TO_S64(int width, unsigned char *bytes, int size);
extern void HIBUF_CHECK(const hibuf_meta_t *meta, const char *N, const char *F, const char *M, const long L);

extern size_t hibuf_byte_length(const hibuf_meta_t * meta, void *object);
extern int    hibuf_byte_encode(const hibuf_meta_t * meta, void *object, char **bytes);
extern void   hibuf_byte_free(char *bytes);

extern int    hibuf_encode(const hibuf_meta_t * meta, void *object, char *bytes, size_t size);
extern int    hibuf_decode(const hibuf_meta_t * meta, char *bytes, size_t size, void **object);
extern void   hibuf_free(const hibuf_meta_t * meta, void *object);

extern const  hibuf_field_t * hibuf_find_field(const hibuf_meta_t * meta, const int id);

#ifdef __cplusplus
}
#endif

#endif

