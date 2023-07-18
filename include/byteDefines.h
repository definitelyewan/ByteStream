#ifndef BYTE_DEFINE
#define BYTE_DEFINE

#ifdef __cplusplus
extern "C"{
#endif

#include <stddef.h>
#include <errno.h>

//ISO_8859_1 encoding
#define ISO_8859_1 0x00

//utf16 encoding
#define UTF16LE 0x01

//utf16be encoding
#define UTF16BE 0x02

//utf8 encoding
#define UTF8 0x03

//uft32 encoding
#define UTF32 0x04

//ascii encoding
#define ASCII 0x05

//unkown encoding
#define OTHER_ENCODING 0xff




#ifdef __cplusplus
} //extern c end
#endif

#endif