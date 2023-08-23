/**
 * @file byteDefines.h
 * @author Ewan Jones
 * @brief preprocessor macros 
 * @version 1.0.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef BYTE_DEFINE
#define BYTE_DEFINE

#ifdef __cplusplus
extern "C"{
#endif

//! Padding for allocating to ensure readers stop without error
#define BYTE_PADDING 2

//! Latin1(ISO_8859_1) encoding
#define BYTE_ISO_8859_1 0x00

//! Utf16le encoding
#define BYTE_UTF16LE 0x01

//! Utf16be encoding
#define BYTE_UTF16BE 0x02

//! Utf8 encoding
#define BYTE_UTF8 0x03

//! Ascii encoding
#define BYTE_ASCII 0x04

//! Unkown encoding
#define BYTE_OTHER_ENCODING 0xff

//! Max horspool table size
#define BYTE_HORSPOOL_TABLE_SIZE 256

//! Size of the byte order mark used by utf16le and be
#define BYTE_BOM_SIZE 2

#ifdef __cplusplus
} //extern c end
#endif

#endif