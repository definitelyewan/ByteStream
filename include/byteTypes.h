#ifndef BYTE_TYPES
#define BYTE_TYPES

#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>
#include "byteDefines.h"

typedef struct _ByteStream{
    unsigned char *buffer;
    size_t bufferSize;
    size_t cursor;
}ByteStream;

#ifdef __cplusplus
} //extern c end
#endif

#endif