# hibuf
Provide happy api for TLV in C

# build
    make clean;make

# run
    ./test.out
    
# usage

**1. Mapping Objec to Meta**

```c
   #include "hibuf.h"
   
   //define struct type 
   typedef struct __elem__ {
        int8_t  s8;
        int16_t s16;
        int32_t s32;
        int64_t s64;
        char    str[4];
   } elem_t;
   
   //delare meta type
   static hibuf_meta_t meta_elem_t;
  
   //mapping relationship
   HIBUF_MAPPING(meta_elem_t, elem_t,
            HIBUF_FIELD_S8(7, s8),
            HIBUF_FIELD_S16(8, s16),
            HIBUF_FIELD_S32(9, s32),
            HIBUF_FIELD_S64(10, s64),
            HIBUF_FIELD_STRING(11,str),
    );
    
    see more test.c
    ...
```  

**2. All Of Types**

```c
    HIBUF_FIELD_S8      -> int8_t
    HIBUF_FIELD_S16     -> int16_t
    HIBUF_FIELD_S32     -> int32_t
    HIBUF_FIELD_S64     -> int64_t
   
    HIBUF_FIELD_U8      -> uint8_t
    HIBUF_FIELD_U16     -> uint16_t
    HIBUF_FIELD_U32     -> uint32_t
    HIBUF_FIELD_U64     -> uint64_t
    
    HIBUF_FIELD_FLOAT   -> float
    HIBUF_FIELD_DOUBLE  -> double
    HIBUF_FIELD_TIME    -> time_t
    
    HIBUF_FIELD_STRING  -> char[]
    HIBUF_FIELD_OBJECT  -> struct foo
    HIBUF_FIELD_ARRAY   -> struct foo[]
        
```

**3. Create & Destroy TLV Buffer**

```c
    extern int    hibuf_byte_encode(const hibuf_meta_t * meta, void *object, char **bytes);
    extern void   hibuf_byte_free(char *bytes);
 ```  
 
**4. Convert to Object From TLV Buffer And Destroy Object**

```c
    extern int    hibuf_decode(const hibuf_meta_t * meta, char *bytes, size_t size, void **object);
    extern void   hibuf_free(const hibuf_meta_t * meta, void *object);
```  


# encode format

```
TLV = {TYPE=2byte, LENGTH=2byte, VALUE=data}

hex dump:
00 01 00 01 01 00 02 00 01 02 00 03 00 01 03 //{00 01[T], 00 01[L], 01[V]}; { 00 02[T], 00 01[L], 02[V]}; { 00 03[T], 00 01[L], 03[V]};
00 04 00 01 04 ...                           //{00 04[T], 00 01[L] 04[V]};
```
