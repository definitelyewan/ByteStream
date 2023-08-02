#ifndef BYTE_UNICODE
#define BYTE_UNICODE

#ifdef __cplusplus
extern "C"{
#endif

#include <byteTypes.h>

//system
void byteSetEndianess(bool endianess);

bool byteIsUtf8(const unsigned char *utf);
size_t byteStrlen(unsigned char encoding, const unsigned char *bytes);
bool byteIsLegalEncoding(unsigned char encoding);
bool byteConvertTextFormat(unsigned char *in, unsigned char inEncoding, size_t inLen, unsigned char **out, unsigned char outEncoding, size_t *outLen);

//utf16
size_t byteUtf16Strlen(const unsigned char *utf);
bool byteHasBOM(const unsigned char *utf);
bool bytePrependBOM(unsigned char **utf, size_t *utfLen);

/**
 * the following functions are heavily inspired by libxml2 the referance
 * can be found at the following link:
 * https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwidoOa53ZaAAxVag4kEHVTXDzMQFnoECBwQAQ&url=https%3A%2F%2Fdev.w3.org%2FXML%2Fencoding.c&usg=AOvVaw2LrNRpxyvCE5_LxXUyIeMC&opi=89978449 
**/

bool byteAsciiToUtf8(unsigned char* out, size_t *outlen, const unsigned char* in, size_t *inlen);
bool byteUtf8ToAscii(unsigned char* out, size_t *outlen, const unsigned char* in, size_t *inlen);
bool byteLat1ToUtf8(unsigned char* out, size_t *outlen, const unsigned char* in, size_t *inlen);
bool byteUtf8ToLat1(unsigned char* out, size_t *outlen, const unsigned char* in, size_t *inlen);
bool byteUtf16leToUtf8(unsigned char* out, size_t *outlen, const unsigned char* inb, size_t *inlenb);
bool byteUtf8ToUtf16le(unsigned char* outb, size_t *outlen, const unsigned char* in, size_t *inlen);
bool byteUtf16beToUtf8(unsigned char* out, size_t *outlen, const unsigned char* inb, size_t *inlenb);
bool byteUtf8ToUtf16be(unsigned char* outb, size_t *outlen, const unsigned char* in, size_t *inlen);

#ifdef __cplusplus
} //extern c end
#endif

#endif