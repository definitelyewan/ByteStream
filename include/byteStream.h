/**
 * @file byteStream.h
 * @author Ewan Jones
 * @brief declerations for stream functions
 * @version 1.0.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef BYTE_STREAM
#define BYTE_STREAM

#ifdef __cplusplus
extern "C"{
#endif

#include <stddef.h>
#include "byteTypes.h"

//mem functions
ByteStream *byteStreamFromFile(const char *fileName);
ByteStream *byteStreamCreate(uint8_t *buffer, size_t bufferSize);
void byteStreamResize(ByteStream *stream, size_t n);
void byteStreamFree(ByteStream *toDelete);
void byteStreamDestroy(ByteStream *toDelete);

//move
bool byteStreamRead(ByteStream *stream, uint8_t *dest, size_t size);
bool byteStreamSeek(ByteStream *stream, size_t dest, const int seekOption);
uint8_t *byteStreamReadUntil(ByteStream *stream, uint8_t delimiter);
void byteStreamSearchAndReplace(ByteStream *stream, uint8_t *pattern, size_t patternSize, uint8_t *replace, size_t replaceSize); //named by mina
uint8_t *byteStreamCursor(ByteStream *stream);
int byteStreamGetCh(ByteStream *stream);
bool byteStreamWrite(ByteStream *stream, uint8_t *src, size_t n);
bool byteStreamWriteAtPosition(ByteStream *stream, uint8_t *src, size_t n, size_t pos);

int byteStreamReadBit(ByteStream *stream, unsigned int k);
bool byteStreamWriteBit(ByteStream *stream, bool bit, unsigned int k);

bool byteStreamDeleteCh(ByteStream *stream);

//returns
uint8_t *byteStreamReturnAscii(ByteStream *stream, size_t *outLen);
uint8_t *byteStreamReturnLatin1(ByteStream *stream, size_t *outLen);
uint8_t *byteStreamReturnUtf8(ByteStream *stream, size_t *outLen);
uint8_t *byteStreamReturnUtf16(ByteStream *stream, size_t *outLen);
int byteStreamReturnInt(ByteStream *stream);
unsigned int byteStreamReturnSyncInt(ByteStream *stream);
size_t byteStreamReturnSize_t(ByteStream *stream);
uint32_t byteStreamReturnU32(ByteStream *stream);

//util
void byteStreamPrintf(const char *formatSpecifier, ByteStream *stream);
bool byteStreamRewind(ByteStream *stream);
long int byteStreamTell(ByteStream *stream);


#ifdef __cplusplus
} //extern c end
#endif

#endif