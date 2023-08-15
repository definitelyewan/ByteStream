#ifndef BYTE_STREAM
#define BYTE_STREAM

#ifdef __cplusplus
extern "C"{
#endif

#include "byteTypes.h"

//mem functions
ByteStream *byteStreamFromFile(const char *fileName);
ByteStream *byteStreamCreate(unsigned char *buffer, size_t bufferSize);
void byteStreamResize(ByteStream *stream, size_t newBufferSize);
void byteStreamFree(ByteStream *toDelete);
void byteStreamDestroy(ByteStream *toDelete);

//move
bool byteStreamRead(ByteStream *stream, unsigned char *dest, size_t size);
bool byteStreamSeek(ByteStream *stream, size_t dest, const int seekOption);
unsigned char *byteStreamReadUntil(ByteStream *stream, unsigned char delimiter);
void byteStreamSearchAndReplace(ByteStream *stream, unsigned char *pattern, size_t patternSize, unsigned char *replace, size_t replaceSize); //named by mina
unsigned char *byteStreamCursor(ByteStream *stream);
int byteStreamGetCh(ByteStream *stream);
bool byteStreamWrite(ByteStream *stream, unsigned char *src, size_t srcSize);
bool byteStreamWriteAtPosition(ByteStream *stream, unsigned char *src, size_t srcSize, size_t pos);

//returns
unsigned char *byteStreamReturnAscii(ByteStream *stream, size_t *outLen);
unsigned char *byteStreamReturnLatin1(ByteStream *stream, size_t *outLen);
unsigned char *byteStreamReturnUtf8(ByteStream *stream, size_t *outLen);
unsigned char *byteStreamReturnUtf16(ByteStream *stream, size_t *outLen);

//util
void byteStreamPrintf(const char *formatSpecifier, ByteStream *stream);


#ifdef __cplusplus
} //extern c end
#endif

#endif