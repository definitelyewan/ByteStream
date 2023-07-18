#ifndef BYTE_TYPES
#define BYTE_TYPES

#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>



typedef struct _ByteStream{
    unsigned char *buffer;
    size_t bufferSize;
    size_t cursor;
}ByteStream;

typedef struct _EncodedStr{
    unsigned char encoding;
    unsigned char *str;
    size_t strSize;
}EncodedStr;

#ifdef __cplusplus
} //extern c end
#endif

#endif