#ifndef BYTE_DEFINE
#define BYTE_DEFINE

#ifdef __cplusplus
extern "C"{
#endif

#include <stddef.h>
#include <errno.h>

//padding for allocating to ensure readers stop without error
#define BYTE_PADDING 2

//ISO_8859_1 encoding
#define BYTE_ISO_8859_1 0x00

//utf16 encoding
#define BYTE_UTF16LE 0x01

//utf16be encoding
#define BYTE_UTF16BE 0x02

//utf8 encoding
#define BYTE_UTF8 0x03

//ascii encoding
#define BYTE_ASCII 0x04

//unkown encoding
#define BYTE_OTHER_ENCODING 0xff

//max horspool table size
#define BYTE_HORSPOOL_TABLE_SIZE 256

//size of the byte order mark used by utf16le and be
#define BYTE_BOM_SIZE 2

#ifdef __cplusplus
} //extern c end
#endif

#endif