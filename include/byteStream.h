#ifndef BYTE_STREAM
#define BYTE_STREAM

#ifdef __cplusplus
extern "C"{
#endif

#include "byteTypes.h"

//mem functions
ByteStream *byteStreamCreate(unsigned char *buffer, size_t bufferSize);
void byteStreamFree(ByteStream *toDelete);
void byteStreamDestroy(ByteStream *toDelete);

//move
void byteStreamRead(ByteStream *stream, unsigned char *dest, size_t size);
void byteStreamSeek(ByteStream *stream, size_t dest, const int seekOption);
void byteStreamReadUntil(ByteStream *stream, unsigned char *dest, unsigned char delimiter);
unsigned char *byteStreamCursor(ByteStream *stream);
unsigned char byteStreamGetCh(ByteStream *stream);




#ifdef __cplusplus
} //extern c end
#endif

#endif