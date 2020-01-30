# hibuf
Provide happy api for TLV in C

# build
    make clean;make

# run
    ./test.out
    
# usage api

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
    
    ...
```   

**2. Create & Destroy TLV Buffer**

```c
    extern int    hibuf_byte_encode(const hibuf_meta_t * meta, void *object, char **bytes);
    extern void   hibuf_byte_free(char *bytes);
 ```  
 
**3. Convert to Object From TLV Buffer And Destroy Object**

```c
    extern int    hibuf_decode(const hibuf_meta_t * meta, char *bytes, size_t size, void **object);
    extern void   hibuf_free(const hibuf_meta_t * meta, void *object);
```  
