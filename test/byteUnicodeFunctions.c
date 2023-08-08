#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include <byteUnicode.h>


size_t fileLen(FILE *fp);

size_t fileLen(FILE *fp){

    size_t s = 0;

    if(fp == NULL){
        return s;
    }

    fseek(fp, 0l, SEEK_END);
    s = ftell(fp);
    fseek(fp, 0l, SEEK_SET);

    return s;
}

static void byteIsUtf8_W3_Setup(void **state){
    (void) state; /* unused */

    unsigned char *b = NULL;
    size_t s = 0;
    FILE *fp = NULL;

    fp = fopen("utf8-complex.txt","rb");
    assert_non_null(fp);
    s = fileLen(fp);
    assert_int_not_equal(s, 0);

    b = calloc(sizeof(unsigned char), s + 1);
    fread(b, sizeof(unsigned char), s, fp);
    fclose(fp);

    bool v = byteIsUtf8(b);
    free(b);
    assert_true(v);
}

static void byteIsUtf8_WithUtf16_Setup(void **state){
    (void) state; /* unused */

    unsigned char *b = NULL;
    size_t s = 0;
    FILE *fp = NULL;

    fp = fopen("utf16.txt","rb");
    assert_non_null(fp);
    s = fileLen(fp);
    assert_int_not_equal(s, 0);

    b = calloc(sizeof(unsigned char), s + 1);
    fread(b, sizeof(unsigned char), s, fp);
    fclose(fp);

    bool v = byteIsUtf8(b);
    free(b);
    assert_false(v);
}

static void byteUtf16Strlen_ForUtf16Le_Setup(void **state){
    (void) state; /* unused */

    unsigned char *b = NULL;
    size_t s = 0;
    size_t l = 0;
    FILE *fp = NULL;

    fp = fopen("utf16.txt","rb");
    assert_non_null(fp);
    s = fileLen(fp);
    assert_int_not_equal(s, 0);

    b = calloc(sizeof(unsigned char), s + BYTE_PADDING);
    fread(b, sizeof(unsigned char), s, fp);
    fclose(fp);

    l = byteUtf16Strlen(b);
    assert_int_equal(s,l);
    
    free(b);
}

static void byteUtf16Strlen_ForUtf16Be_Setup(void **state){
    (void) state; /* unused */

    unsigned char *b = NULL;
    size_t s = 0;
    size_t l = 0;
    FILE *fp = NULL;

    fp = fopen("utf16.txt","rb");
    assert_non_null(fp);
    s = fileLen(fp);
    assert_int_not_equal(s, 0);

    b = calloc(sizeof(unsigned char), s + BYTE_PADDING);
    fread(b, sizeof(unsigned char), s, fp);
    fclose(fp);

    //to be
    for(size_t i = 0; i < s;){
        unsigned char tmp = b[i];

        b[i] = b[i+1];
        b[i+1] = tmp;
        i += 2;
    }

    l = byteUtf16Strlen(b);
    assert_int_equal(s,l);
    free(b);
}

static void byteUtf16Strlen_ForNull_Setup(void **state){
    (void) state; //unused
    size_t s = 0;

    s = byteUtf16Strlen(NULL);
    assert_int_equal(s,0);

}

static void byteStrlen_Latin1_Setup(void **state){
    (void) state; //unused
    
    unsigned char *b = NULL;
    size_t s = 0;
    size_t l = 0;
    FILE *fp = NULL;

    fp = fopen("latin1.txt","rb");
    assert_non_null(fp);
    s = fileLen(fp);
    assert_int_not_equal(s, 0);

    b = calloc(sizeof(unsigned char), s + 1);
    fread(b, sizeof(unsigned char), s, fp);
    fclose(fp);

    l = byteStrlen(BYTE_ISO_8859_1, b);
    assert_int_equal(s,l);
    free(b);

}

static void byteStrlen_Ascii_Setup(void **state){
    (void) state; //unused
    
    unsigned char *b = NULL;
    size_t s = 0;
    size_t l = 0;
    FILE *fp = NULL;

    fp = fopen("ascii.txt","rb");
    assert_non_null(fp);
    s = fileLen(fp);
    assert_int_not_equal(s, 0);

    b = calloc(sizeof(unsigned char), s + 1);
    fread(b, sizeof(unsigned char), s, fp);
    fclose(fp);

    l = byteStrlen(BYTE_ASCII, b);
    assert_int_equal(s,l);
    free(b);
}

static void byteStrlen_Utf8_Setup(void **state){
    (void) state; //unused
    
    unsigned char *b = NULL;
    size_t s = 0;
    size_t l = 0;
    FILE *fp = NULL;

    fp = fopen("utf8-complex.txt","rb");
    assert_non_null(fp);
    s = fileLen(fp);
    assert_int_not_equal(s, 0);

    b = calloc(sizeof(unsigned char), s + 1);
    fread(b, sizeof(unsigned char), s, fp);
    fclose(fp);

    l = byteStrlen(BYTE_UTF8, b);
    assert_int_equal(s,l);
    free(b);
}

static void byteStrlen_Utf16Le_Setup(void **state){
    (void) state; //unused
    
    unsigned char *b = NULL;
    size_t s = 0;
    size_t l = 0;
    FILE *fp = NULL;

    fp = fopen("utf16.txt","rb");
    assert_non_null(fp);
    s = fileLen(fp);
    assert_int_not_equal(s, 0);

    b = calloc(sizeof(unsigned char), s + BYTE_PADDING);
    fread(b, sizeof(unsigned char), s, fp);
    fclose(fp);

    l = byteStrlen(BYTE_UTF16LE, b);
    assert_int_equal(s,l);
    free(b);
}

static void byteStrlen_Utf16Be_Setup(void **state){
    (void) state; //unused
    
    unsigned char *b = NULL;
    size_t s = 0;
    size_t l = 0;
    FILE *fp = NULL;

    fp = fopen("utf16.txt","rb");
    assert_non_null(fp);
    s = fileLen(fp);
    assert_int_not_equal(s, 0);

    b = calloc(sizeof(unsigned char), s + BYTE_PADDING);
    fread(b, sizeof(unsigned char), s, fp);
    fclose(fp);

    //to be
    for(size_t i = 0; i < s;){
        unsigned char tmp = b[i];

        b[i] = b[i+1];
        b[i+1] = tmp;
        i += 2;
    }

    l = byteStrlen(BYTE_UTF16BE,b);
    assert_int_equal(s,l);
    free(b);
}

static void byteStrlen_Unknown_Setup(void **state){
    (void) state; //unused
    
    unsigned char *b = NULL;
    size_t s = 0;
    size_t l = 0;
    FILE *fp = NULL;

    fp = fopen("utf16.txt","rb");
    assert_non_null(fp);
    s = fileLen(fp);
    assert_int_not_equal(s, 0);

    b = calloc(sizeof(unsigned char), s + BYTE_PADDING);
    fread(b, sizeof(unsigned char), s, fp);
    fclose(fp);

    l = byteStrlen(90,b);
    assert_int_equal(0,l);
    free(b);
}

static void byteIsLegalEncoding_Latin1_Setup(void **state){
    (void) state; //unused
    
    bool v = byteIsLegalEncoding(BYTE_ISO_8859_1);
    assert_true(v);
}

static void byteIsLegalEncoding_Ascii_Setup(void **state){
    (void) state; //unused
    
    bool v = byteIsLegalEncoding(BYTE_ASCII);
    assert_true(v);
}

static void byteIsLegalEncoding_Utf8_Setup(void **state){
    (void) state; //unused
    
    bool v = byteIsLegalEncoding(BYTE_UTF8);
    assert_true(v);
}

static void byteIsLegalEncoding_Utf16Be_Setup(void **state){
    (void) state; //unused
    
    bool v = byteIsLegalEncoding(BYTE_UTF16BE);
    assert_true(v);
}

static void byteyteIsLegalEncoding_Utf16Le_Setup(void **state){
    (void) state; //unused
    
    bool v = byteIsLegalEncoding(BYTE_UTF16LE);
    assert_true(v);
}

static void byteIsLegalEncoding_NoEncoding_Setup(void **state){
    (void) state; //unused
    
    bool v = byteIsLegalEncoding(9);
    assert_false(v);
}

static void byteHasBOM_DoesHaveBOM_Setup(void **state){
    (void) state; //unused

    unsigned char a[2] = {0xff,0xfe};
    unsigned char b[2] = {0xfe,0xff};

    assert_true(byteHasBOM(a));
    assert_true(byteHasBOM(b));
}

static void byteHasBOM_DoesNotHaveBOM_Setup(void **state){
    (void) state; //unused

    unsigned char a[2] = {0x00,0x00};

    assert_false(byteHasBOM(a));
}

static void bytePrependBOM_FFFE_Setup(void **state){
    (void) state; //unused

    unsigned char *pass = calloc(1,8);
    strcpy((char *)pass, "bomfffe");

    size_t s = strlen((char *)pass);
    size_t oldS = s;
    bool v = bytePrependBOM(BYTE_UTF16LE, &pass, &s);

    assert_true(v);
    assert_int_not_equal(oldS, s);
    assert_memory_equal("\xff\xfe",pass,2);
    free(pass);
}

static void bytePrependBOM_FEFF_Setup(void **state){
    (void) state; //unused

    unsigned char *pass = calloc(1,8);
    strcpy((char *)pass, "bomfeff");

    size_t s = strlen((char *)pass);
    size_t oldS = s;
    pass[0] = '\0';

    bool v = bytePrependBOM(BYTE_UTF16BE, &pass, &s);

    assert_true(v);
    assert_int_not_equal(oldS, s);
    assert_memory_equal("\xfe\xff",pass,2);
    free(pass);
}

static void bytePrependBOM_Err_Setup(void **state){
    (void) state; //unused

    size_t s = 1;
    bool v = bytePrependBOM(9, NULL, &s);

    assert_false(v);
}

static void byteConvertTextFormat_ConvertErr_Setup(void **state){
    (void) state; //unused

    unsigned char *in = NULL;
    unsigned char **out = NULL;
    size_t inl = 0;
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, 9, inl, out, 9, &outl);
    assert_false(v);

    //value would of changed if any conversion happend
    assert_int_equal(outl,0);
}

static void byteConvertTextFormat_ConvertAsciiToLatin1_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "ascii to Latin1 test";
    unsigned char expected[] = "ascii to Latin1 test";

    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_ASCII, inl, &out, BYTE_ISO_8859_1, &outl);

    assert_true(v);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertAsciiToLatin1V2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "a\rsc\0ii t\'o Lat\"in1 te\nst";
    unsigned char expected[] = "a\rsc\0ii t\'o Lat\"in1 te\nst";

    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_ASCII, inl, &out, BYTE_ISO_8859_1, &outl);

    assert_true(v);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertAsciiToUtf16Le_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "ascii to utf16le test";
    unsigned char expected[] = {
     'a', 0, 's', 0, 'c', 0, 'i', 0, 'i', 0, ' ', 0, 't', 0, 'o', 0, ' ', 0, 
     'u', 0, 't', 0, 'f', 0, '1', 0, '6', 0, 'l', 0, 'e', 0, ' ', 0, 't', 0, 
     'e', 0, 's', 0, 't', 0};

    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_ASCII, inl, &out, BYTE_UTF16LE, &outl);

    assert_true(v);
    assert_memory_equal(expected,out,outl);
    free(out);

}

static void byteConvertTextFormat_ConvertAsciiToUtf16LeV2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "Hello, World!";
    unsigned char expected[] = {
     'H', 0, 'e', 0, 'l', 0, 'l', 0, 'o', 0, ',', 0, ' ', 0, 'W', 0, 'o', 0, 'r',
      0, 'l', 0, 'd', 0, '!', 0};

    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_ASCII, inl, &out, BYTE_UTF16LE, &outl);

    assert_true(v);
    assert_memory_equal(expected, out, outl);
    free(out);
}

static void byteConvertTextFormat_ConvertAsciiToUtf16Be_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "ascii to utf16le test";
    unsigned char expected[] = {
     0, 'a', 0, 's', 0, 'c', 0, 'i', 0, 'i', 0, ' ', 0, 't', 0, 'o', 0, ' ', 
     0, 'u', 0, 't', 0, 'f', 0, '1', 0, '6', 0, 'l', 0, 'e', 0, ' ', 0, 't', 
     0, 'e', 0, 's', 0, 't'};

    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_ASCII, inl, &out, BYTE_UTF16BE, &outl);

    assert_true(v);
    assert_memory_equal(expected,out,outl);
    free(out);
}

static void byteConvertTextFormat_ConvertAsciiToUtf16BeV2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "Hello, World!";
    unsigned char expected[] = {
     0, 'H', 0, 'e', 0, 'l', 0, 'l', 0, 'o', 0, ',', 0, ' ', 0, 'W', 0, 'o', 0,
     'r', 0, 'l', 0, 'd', 0, '!'};

    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_ASCII, inl, &out, BYTE_UTF16BE, &outl);

    assert_true(v);
    assert_memory_equal(expected, out, outl);
    free(out);
}

static void byteConvertTextFormat_ConvertAsciiToUtf8_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "ascii to utf8 test";
    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_ASCII, inl, &out, BYTE_UTF8, &outl);

    assert_true(v);
    assert_string_equal((char *)in,(char *)out);
    free(out);
}

static void byteConvertTextFormat_ConvertAsciiToUtf8V2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "ascii to utf8 test\nwith a control\rcharacter in it \t";
    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_ASCII, inl, &out, BYTE_UTF8, &outl);

    assert_true(v);
    assert_string_equal((char *)out, (char *)in);
    free(out);
}

static void byteConvertTextFormat_ConvertLatin1ToAscii_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "\xAC¡not going to work!\xAE";
    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    bool v = byteConvertTextFormat(in, BYTE_ISO_8859_1, inl, &out, BYTE_ASCII, &outl);

    assert_false(v);
    assert_int_equal(outl, 0);
    assert_null(out);
}

static void byteConvertTextFormat_ConvertLatin1ToAsciiV2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "latin1 to ascii";
    unsigned char expected[] = "latin1 to ascii";
    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    bool v = byteConvertTextFormat(in, BYTE_ISO_8859_1, inl, &out, BYTE_ASCII, &outl);

    assert_true(v);
    assert_int_equal(outl, inl);
    assert_string_equal((char *)expected,(char*)in);
    free(out);
}

static void byteConvertTextFormat_ConvertLatin1ToUtf16Le_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "\xAC\xA1latin1 to utf16le !\xAE";
    unsigned char expected[] = {
     0xAC, 0, 0xA1, 0, 'l', 0, 'a', 0, 't', 0, 'i', 0, 'n', 0, '1', 0, ' ', 0, 
     't', 0, 'o', 0, ' ', 0, 'u', 0, 't', 0, 'f', 0, '1', 0, '6', 0, 'l', 0, 
     'e', 0, ' ', 0, '!', 0, 0xAE, 0};
    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    bool v = byteConvertTextFormat(in, BYTE_ISO_8859_1, inl, &out, BYTE_UTF16LE, &outl);

    assert_true(v);
    assert_int_not_equal(outl, 0);
    assert_memory_equal(expected,out,outl);
    free(out);
}

static void byteConvertTextFormat_ConvertLatin1ToUtf16LeV2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = {
     1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
     21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 
     39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 
     57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 
     75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 
     93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 
     109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 
     124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 
     139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 
     154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 
     169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183,
     184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
     199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 
     214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 
     229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 
     244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 0};
    unsigned char expected[] = {
     1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0,
     13, 0, 14, 0, 15, 0, 16, 0, 17, 0, 18, 0, 19, 0, 20, 0, 21, 0, 22, 0, 23, 0, 
     24, 0, 25, 0, 26, 0, 27, 0, 28, 0, 29, 0, 30, 0, 31, 0, 32, 0, 33, 0, 34, 0, 
     35, 0, 36, 0, 37, 0, 38, 0, 39, 0, 40, 0, 41, 0, 42, 0, 43, 0, 44, 0, 45, 0, 
     46, 0, 47, 0, 48, 0, 49, 0, 50, 0, 51, 0, 52, 0, 53, 0, 54, 0, 55, 0, 56, 0, 
     57, 0, 58, 0, 59, 0, 60, 0, 61, 0, 62, 0, 63, 0, 64, 0, 65, 0, 66, 0, 67, 0, 
     68, 0, 69, 0, 70, 0, 71, 0, 72, 0, 73, 0, 74, 0, 75, 0, 76, 0, 77, 0, 78, 0,
     79, 0, 80, 0, 81, 0, 82, 0, 83, 0, 84, 0, 85, 0, 86, 0, 87, 0, 88, 0, 89, 0, 
     90, 0, 91, 0, 92, 0, 93, 0, 94, 0, 95, 0, 96, 0, 97, 0, 98, 0, 99, 0, 100, 0, 
     101, 0, 102, 0, 103, 0, 104, 0, 105, 0, 106, 0, 107, 0, 108, 0, 109, 0, 110, 0, 
     111, 0, 112, 0, 113, 0, 114, 0, 115, 0, 116, 0, 117, 0, 118, 0, 119, 0, 120, 0, 
     121, 0, 122, 0, 123, 0, 124, 0, 125, 0, 126, 0, 127, 0, 128, 0, 129, 0, 130, 0, 
     131, 0, 132, 0, 133, 0, 134, 0, 135, 0, 136, 0, 137, 0, 138, 0, 139, 0, 140, 0, 
     141, 0, 142, 0, 143, 0, 144, 0, 145, 0, 146, 0, 147, 0, 148, 0, 149, 0, 150, 0, 
     151, 0, 152, 0, 153, 0, 154, 0, 155, 0, 156, 0, 157, 0, 158, 0, 159, 0, 160, 0, 
     161, 0, 162, 0, 163, 0, 164, 0, 165, 0, 166, 0, 167, 0, 168, 0, 169, 0, 170, 0, 
     171, 0, 172, 0, 173, 0, 174, 0, 175, 0, 176, 0, 177, 0, 178, 0, 179, 0, 180, 0, 
     181, 0, 182, 0, 183, 0, 184, 0, 185, 0, 186, 0, 187, 0, 188, 0, 189, 0, 190, 0, 
     191, 0, 192, 0, 193, 0, 194, 0, 195, 0, 196, 0, 197, 0, 198, 0, 199, 0, 200, 0, 
     201, 0, 202, 0, 203, 0, 204, 0, 205, 0, 206, 0, 207, 0, 208, 0, 209, 0, 210, 0, 
     211, 0, 212, 0, 213, 0, 214, 0, 215, 0, 216, 0, 217, 0, 218, 0, 219, 0, 220, 0, 
     221, 0, 222, 0, 223, 0, 224, 0, 225, 0, 226, 0, 227, 0, 228, 0, 229, 0, 230, 0, 
     231, 0, 232, 0, 233, 0, 234, 0, 235, 0, 236, 0, 237, 0, 238, 0, 239, 0, 240, 0, 
     241, 0, 242, 0, 243, 0, 244, 0, 245, 0, 246, 0, 247, 0, 248, 0, 249, 0, 250, 0, 
     251, 0, 252, 0, 253, 0, 254, 0, 255, 0};
    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;

    bool v = byteConvertTextFormat(in, BYTE_ISO_8859_1, inl, &out, BYTE_UTF16LE, &outl);

    assert_true(v);
    assert_int_not_equal(outl, 0);
    assert_memory_equal(expected,out,outl);
    free(out);
}

static void byteConvertTextFormat_ConvertLatin1ToUtf16Be_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = {
     1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
     21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 
     39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 
     57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 
     75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 
     93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 
     109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 
     124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 
     139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 
     154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 
     169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183,
     184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
     199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 
     214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 
     229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 
     244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 0};
    unsigned char expected[] = {
     0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0,
     13, 0, 14, 0, 15, 0, 16, 0, 17, 0, 18, 0, 19, 0, 20, 0, 21, 0, 22, 0, 23, 0, 
     24, 0, 25, 0, 26, 0, 27, 0, 28, 0, 29, 0, 30, 0, 31, 0, 32, 0, 33, 0, 34, 0, 
     35, 0, 36, 0, 37, 0, 38, 0, 39, 0, 40, 0, 41, 0, 42, 0, 43, 0, 44, 0, 45, 0, 
     46, 0, 47, 0, 48, 0, 49, 0, 50, 0, 51, 0, 52, 0, 53, 0, 54, 0, 55, 0, 56, 0, 
     57, 0, 58, 0, 59, 0, 60, 0, 61, 0, 62, 0, 63, 0, 64, 0, 65, 0, 66, 0, 67, 0, 
     68, 0, 69, 0, 70, 0, 71, 0, 72, 0, 73, 0, 74, 0, 75, 0, 76, 0, 77, 0, 78, 0, 
     79, 0, 80, 0, 81, 0, 82, 0, 83, 0, 84, 0, 85, 0, 86, 0, 87, 0, 88, 0, 89, 0, 
     90, 0, 91, 0, 92, 0, 93, 0, 94, 0, 95, 0, 96, 0, 97, 0, 98, 0, 99, 0, 100, 0, 
     101, 0, 102, 0, 103, 0, 104, 0, 105, 0, 106, 0, 107, 0, 108, 0, 109, 0, 110, 0, 
     111, 0, 112, 0, 113, 0, 114, 0, 115, 0, 116, 0, 117, 0, 118, 0, 119, 0, 120, 0, 
     121, 0, 122, 0, 123, 0, 124, 0, 125, 0, 126, 0, 127, 0, 128, 0, 129, 0, 130, 0, 
     131, 0, 132, 0, 133, 0, 134, 0, 135, 0, 136, 0, 137, 0, 138, 0, 139, 0, 140, 0, 
     141, 0, 142, 0, 143, 0, 144, 0, 145, 0, 146, 0, 147, 0, 148, 0, 149, 0, 150, 0, 
     151, 0, 152, 0, 153, 0, 154, 0, 155, 0, 156, 0, 157, 0, 158, 0, 159, 0, 160, 0, 
     161, 0, 162, 0, 163, 0, 164, 0, 165, 0, 166, 0, 167, 0, 168, 0, 169, 0, 170, 0, 
     171, 0, 172, 0, 173, 0, 174, 0, 175, 0, 176, 0, 177, 0, 178, 0, 179, 0, 180, 0, 
     181, 0, 182, 0, 183, 0, 184, 0, 185, 0, 186, 0, 187, 0, 188, 0, 189, 0, 190, 0, 
     191, 0, 192, 0, 193, 0, 194, 0, 195, 0, 196, 0, 197, 0, 198, 0, 199, 0, 200, 0, 
     201, 0, 202, 0, 203, 0, 204, 0, 205, 0, 206, 0, 207, 0, 208, 0, 209, 0, 210, 0, 
     211, 0, 212, 0, 213, 0, 214, 0, 215, 0, 216, 0, 217, 0, 218, 0, 219, 0, 220, 0, 
     221, 0, 222, 0, 223, 0, 224, 0, 225, 0, 226, 0, 227, 0, 228, 0, 229, 0, 230, 0, 
     231, 0, 232, 0, 233, 0, 234, 0, 235, 0, 236, 0, 237, 0, 238, 0, 239, 0, 240, 0, 
     241, 0, 242, 0, 243, 0, 244, 0, 245, 0, 246, 0, 247, 0, 248, 0, 249, 0, 250, 0, 
     251, 0, 252, 0, 253, 0, 254, 0, 255, 0};
    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;

    bool v = byteConvertTextFormat(in, BYTE_ISO_8859_1, inl, &out, BYTE_UTF16BE, &outl);

    assert_true(v);
    assert_int_not_equal(outl, 0);
    assert_memory_equal(expected,out,outl);
    free(out);
}

static void byteConvertTextFormat_ConvertLatin1ToUtf16BeV2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = {
     160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
     176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
     0};
    unsigned char expected[] = {
     0, 160, 0, 161, 0, 162, 0, 163, 0, 164, 0, 165, 0, 166, 0, 167, 0, 168, 0, 169,
     0, 170, 0, 171, 0, 172, 0, 173, 0, 174, 0, 175, 0, 176, 0, 177, 0, 178, 0, 179,
     0, 180, 0, 181, 0, 182, 0, 183, 0, 184, 0, 185, 0, 186, 0, 187, 0, 188, 0, 189, 
     0, 190, 0, 191, 0};
    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    bool v = byteConvertTextFormat(in, BYTE_ISO_8859_1, inl, &out, BYTE_UTF16BE, &outl);

    assert_true(v);
    assert_int_not_equal(outl, 0);
    assert_memory_equal(expected,out,outl);
    free(out);
}

static void byteConvertTextFormat_ConvertLatin1ToUtf8_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = {188,189,190,0};
    unsigned char expected[] = {0xc2,0xbc,0xc2,0xbd,0xc2,0xbe,0};
    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    bool v = byteConvertTextFormat(in, BYTE_ISO_8859_1, inl, &out, BYTE_UTF8, &outl);

    assert_true(v);
    assert_int_not_equal(outl, 0);
    assert_memory_equal(expected,out,outl);
    free(out);
}

static void byteConvertTextFormat_ConvertLatin1ToUtf8V2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "latin1 to \xdctf8";
    unsigned char expected[] = {0x6c,0x61,0x74,0x69,0x6e,0x31,0x20,0x74,0x6f,0x20,0xc3,0x9c,0x74,0x66,0x38,0};
    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    bool v = byteConvertTextFormat(in, BYTE_ISO_8859_1, inl, &out, BYTE_UTF8, &outl);

    assert_true(v);
    assert_int_not_equal(outl, 0);
    assert_memory_equal(expected,out,outl);
    free(out);
}

static void byteConvertTextFormat_ConvertUtf8ToAscii_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "ascii to Latin1 test";
    unsigned char expected[] = "ascii to Latin1 test";

    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF8, inl, &out, BYTE_ASCII, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf8ToAsciiV2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "s";
    unsigned char expected[] = "s";

    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF8, inl, &out, BYTE_ASCII, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf8ToLatin1_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "ßÓ«]×";
    unsigned char expected[] = {0xDF, 0xD3, 0xAB, ']', 0xD7};

    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF8, inl, &out, BYTE_ISO_8859_1, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf8ToLatin1V2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "\n!\"#$\x25&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~¡¢£¤¥¦§¨©ª«¬®¯°±²³´µ¶·¸¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ";
    unsigned char expected[] = {
     10, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
     50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67,
     68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78 ,79, 80, 81, 82, 83, 84, 85,
     86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103,
     104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118,
     119, 120, 121, 122,123, 124, 125, 126, 161, 162, 163, 164, 165, 166, 167,
     168, 169, 170, 171, 172, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183,
     184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
     199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213,
     214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228,
     229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243,
     244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 0};

    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF8, inl, &out, BYTE_ISO_8859_1, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}


static void byteConvertTextFormat_ConvertUtf8ToUtf16Le_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "हिंदी日本";
    unsigned char expected[] = {0x39, 0x09, 0x3f, 0x09, 0x02, 0x09, 0x26, 0x09, 0x40, 0x09, 0xe5, 0x65, 0x2c, 0x67};

    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF8, inl, &out, BYTE_UTF16LE, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf8ToUtf16LeV2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "اللغة العربية لغة";
    unsigned char expected[] = {
     0x27, 0x06, 0x44, 0x06, 0x44, 0x06, 0x3a, 0x06, 0x29, 0x06, 0x20, 0x00, 
     0x27, 0x06, 0x44, 0x06, 0x39, 0x06, 0x31, 0x06, 0x28, 0x06, 0x4a, 0x06, 
     0x29, 0x06, 0x20, 0x00, 0x44, 0x06, 0x3a, 0x06, 0x29, 0x06};

    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF8, inl, &out, BYTE_UTF16LE, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf8ToUtf16Be_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "हिंदी日本";
    unsigned char expected[] = {0x09, 0x39, 0x09, 0x3f, 0x09, 0x02, 0x09, 0x26, 0x09, 0x40, 0x65, 0xe5, 0x67, 0x2c};

    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF8, inl, &out, BYTE_UTF16BE, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf8ToUtf16BeV2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "اللغة العربية لغة";
    unsigned char expected[] = {
     0x06, 0x27, 0x06, 0x44, 0x06, 0x44, 0x06, 0x3a, 0x06, 0x29, 0x00, 0x20, 0x06, 
     0x27, 0x06, 0x44, 0x06, 0x39, 0x06, 0x31, 0x06, 0x28, 0x06, 0x4a, 0x06, 0x29, 
     0x00, 0x20, 0x06, 0x44, 0x06, 0x3a, 0x06, 0x29};

    unsigned char *out = NULL;
    size_t inl = strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF8, inl, &out, BYTE_UTF16BE, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf16BeToAscii_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = {0, 'a'};
    unsigned char expected[] = {'a', 0};

    unsigned char *out = NULL;
    size_t inl = 2;
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF16BE, inl, &out, BYTE_ASCII, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf16BeToAsciiV2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = {
     0, 'n', 0, 'o', 0, 'w', 0,'\n', 0, 'i', 0, 'n', 0, '\n', 0, 'a', 0, 
     's', 0, 'c', 0, 'i', 0, 'i'};
    unsigned char expected[] = "now\nin\nascii";

    unsigned char *out = NULL;
    size_t inl = 24;
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF16BE, inl, &out, BYTE_ASCII, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf16BeToLatin1_Setup(void **state){
    (void) state; //unused

    unsigned char expected[] = {
     1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
     21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 
     39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 
     57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 
     75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 
     93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 
     109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 
     124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 
     139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 
     154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 
     169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183,
     184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
     199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 
     214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 
     229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 
     244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 0};
    unsigned char in[] = {
     0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0, 10, 0, 11, 0, 12, 0,
     13, 0, 14, 0, 15, 0, 16, 0, 17, 0, 18, 0, 19, 0, 20, 0, 21, 0, 22, 0, 23, 0, 
     24, 0, 25, 0, 26, 0, 27, 0, 28, 0, 29, 0, 30, 0, 31, 0, 32, 0, 33, 0, 34, 0, 
     35, 0, 36, 0, 37, 0, 38, 0, 39, 0, 40, 0, 41, 0, 42, 0, 43, 0, 44, 0, 45, 0, 
     46, 0, 47, 0, 48, 0, 49, 0, 50, 0, 51, 0, 52, 0, 53, 0, 54, 0, 55, 0, 56, 0, 
     57, 0, 58, 0, 59, 0, 60, 0, 61, 0, 62, 0, 63, 0, 64, 0, 65, 0, 66, 0, 67, 0, 
     68, 0, 69, 0, 70, 0, 71, 0, 72, 0, 73, 0, 74, 0, 75, 0, 76, 0, 77, 0, 78, 0, 
     79, 0, 80, 0, 81, 0, 82, 0, 83, 0, 84, 0, 85, 0, 86, 0, 87, 0, 88, 0, 89, 0, 
     90, 0, 91, 0, 92, 0, 93, 0, 94, 0, 95, 0, 96, 0, 97, 0, 98, 0, 99, 0, 100, 0, 
     101, 0, 102, 0, 103, 0, 104, 0, 105, 0, 106, 0, 107, 0, 108, 0, 109, 0, 110, 0, 
     111, 0, 112, 0, 113, 0, 114, 0, 115, 0, 116, 0, 117, 0, 118, 0, 119, 0, 120, 0, 
     121, 0, 122, 0, 123, 0, 124, 0, 125, 0, 126, 0, 127, 0, 128, 0, 129, 0, 130, 0, 
     131, 0, 132, 0, 133, 0, 134, 0, 135, 0, 136, 0, 137, 0, 138, 0, 139, 0, 140, 0, 
     141, 0, 142, 0, 143, 0, 144, 0, 145, 0, 146, 0, 147, 0, 148, 0, 149, 0, 150, 0, 
     151, 0, 152, 0, 153, 0, 154, 0, 155, 0, 156, 0, 157, 0, 158, 0, 159, 0, 160, 0, 
     161, 0, 162, 0, 163, 0, 164, 0, 165, 0, 166, 0, 167, 0, 168, 0, 169, 0, 170, 0, 
     171, 0, 172, 0, 173, 0, 174, 0, 175, 0, 176, 0, 177, 0, 178, 0, 179, 0, 180, 0, 
     181, 0, 182, 0, 183, 0, 184, 0, 185, 0, 186, 0, 187, 0, 188, 0, 189, 0, 190, 0, 
     191, 0, 192, 0, 193, 0, 194, 0, 195, 0, 196, 0, 197, 0, 198, 0, 199, 0, 200, 0, 
     201, 0, 202, 0, 203, 0, 204, 0, 205, 0, 206, 0, 207, 0, 208, 0, 209, 0, 210, 0, 
     211, 0, 212, 0, 213, 0, 214, 0, 215, 0, 216, 0, 217, 0, 218, 0, 219, 0, 220, 0, 
     221, 0, 222, 0, 223, 0, 224, 0, 225, 0, 226, 0, 227, 0, 228, 0, 229, 0, 230, 0, 
     231, 0, 232, 0, 233, 0, 234, 0, 235, 0, 236, 0, 237, 0, 238, 0, 239, 0, 240, 0, 
     241, 0, 242, 0, 243, 0, 244, 0, 245, 0, 246, 0, 247, 0, 248, 0, 249, 0, 250, 0, 
     251, 0, 252, 0, 253, 0, 254, 0, 255, 0};
    unsigned char *out = NULL;
    size_t inl = 255;//strlen((char *)in);
    size_t outl = 0;

    bool v = byteConvertTextFormat(in, BYTE_UTF16BE, inl, &out, BYTE_ISO_8859_1, &outl);

    assert_true(v);
    assert_int_not_equal(outl, 0);
    assert_memory_equal(expected,out,outl);
    free(out);
}

static void byteConvertTextFormat_ConvertUtf16BeToLatin1V2_Setup(void **state){
    (void) state; //unused

    unsigned char expected[] = {
     160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
     176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
     0};
    unsigned char in[] = {
     0, 160, 0, 161, 0, 162, 0, 163, 0, 164, 0, 165, 0, 166, 0, 167, 0, 168, 0, 169,
     0, 170, 0, 171, 0, 172, 0, 173, 0, 174, 0, 175, 0, 176, 0, 177, 0, 178, 0, 179,
     0, 180, 0, 181, 0, 182, 0, 183, 0, 184, 0, 185, 0, 186, 0, 187, 0, 188, 0, 189, 
     0, 190, 0, 191, 0};
    unsigned char *out = NULL;
    size_t inl = 64;//strlen((char *)in);
    size_t outl = 0;
    bool v = byteConvertTextFormat(in, BYTE_UTF16BE, inl, &out, BYTE_ISO_8859_1, &outl);

    assert_true(v);
    assert_int_not_equal(outl, 0);
    assert_memory_equal(expected,out,outl);
    free(out);
}

static void byteConvertTextFormat_ConvertUtf16BeToUtf8_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = {
     0x06, 0x27, 0x06, 0x44, 0x06, 0x44, 0x06, 0x3a, 0x06, 0x29, 0x00, 0x20, 0x06, 
     0x27, 0x06, 0x44, 0x06, 0x39, 0x06, 0x31, 0x06, 0x28, 0x06, 0x4a, 0x06, 0x29, 
     0x00, 0x20, 0x06, 0x44, 0x06, 0x3a, 0x06, 0x29};
    unsigned char expected[] = "اللغة العربية لغة";

    unsigned char *out = NULL;
    size_t inl = 34;//strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF16BE, inl, &out, BYTE_UTF8, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf16BeToUtf8V2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = {0x6c, 0xd5, 0x8b, 0xed, 0x66, 0x5a, 0x4e, 0x0a, 0x59, 0x7d};
    unsigned char expected[] = "法语晚上好";

    unsigned char *out = NULL;
    size_t inl = 12;//strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF16BE, inl, &out, BYTE_UTF8, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf16BeToUtf16Le_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = {
     0x06, 0x27, 0x06, 0x44, 0x06, 0x44, 0x06, 0x3a, 0x06, 0x29, 0x00, 0x20, 0x06, 
     0x27, 0x06, 0x44, 0x06, 0x39, 0x06, 0x31, 0x06, 0x28, 0x06, 0x4a, 0x06, 0x29, 
     0x00, 0x20, 0x06, 0x44, 0x06, 0x3a, 0x06, 0x29, 0, 0, 0, 0};
    unsigned char expected[] = {
     0x27, 0x06, 0x44, 0x06, 0x44, 0x06, 0x3a, 0x06, 0x29, 0x06, 0x20, 0x00, 0x27, 
     0x06, 0x44, 0x06, 0x39, 0x06, 0x31, 0x06, 0x28, 0x06, 0x4a, 0x06, 0x29, 0x06, 
     0x20, 0x00, 0x44, 0x06, 0x3a, 0x06, 0x29, 0x06, 0, 0, 0, 0};

    unsigned char *out = NULL;
    size_t inl = 34;//strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF16BE, inl, &out, BYTE_UTF16LE, &outl);
    
    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf16BeToUtf16LeV2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = {0x6c, 0xd5, 0x8b, 0xed, 0x66, 0x5a, 0x4e, 0x0a, 0x59, 0x7d};
    unsigned char expected[] = {0xd5, 0x6c, 0xed, 0x8b, 0x5a, 0x66, 0x0a, 0x4e, 0x7d, 0x59};
    //"法语晚上好"

    unsigned char *out = NULL;
    size_t inl = 12;//strlen((char *)in);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF16BE, inl, &out, BYTE_UTF16LE, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf16LeToAscii_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = {
     0x49, 0x00, 0x20, 0x00, 0x63, 0x00, 0x61, 0x00, 0x6e, 0x00, 0x27, 0x00, 
     0x74, 0x00, 0x20, 0x00, 0x74, 0x00, 0x65, 0x00, 0x73, 0x00, 0x74, 0x00, 
     0x20, 0x00, 0x6e, 0x00, 0x6f, 0x00, 0x20, 0x00, 0x6d, 0x00, 0x6f, 0x00, 
     0x72, 0x00, 0x65, 0x00, 0x20, 0x00, 0x3a, 0x00, 0x28, 0x00, 0x29, 0x00};
    unsigned char expected[] = "I can't test no more :()";

    unsigned char *out = NULL;
    size_t inl = strlen((char *)expected);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF16LE, inl, &out, BYTE_ASCII, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf16LeToAsciiV2_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = {'\r', 0, '\t', 0, '\n', 0};
    unsigned char expected[] = "\r\t\n";

    unsigned char *out = NULL;
    size_t inl = 6;//strlen((char *)expected);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF16LE, inl, &out, BYTE_ASCII, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf16LeToLatin1_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = {0xdf, 0x00, 0xd3, 0x00, 0xab, 0x00, 0x5d, 0x00, 0xd7, 0x00};
    unsigned char expected[] = {0xDF, 0xD3, 0xAB, ']', 0xD7, 0};
    //ßÓ«]×

    unsigned char *out = NULL;
    size_t inl = 10;//strlen((char *)expected);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF16LE, inl, &out, BYTE_ISO_8859_1, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf16LeToLatin1V2_Setup(void **state){
    (void) state; //unused
    
    //français
    unsigned char in[] = {0x66, 0x00, 0x72, 0x00, 0x61, 0x00, 0x6e, 0x00, 0xe7, 0x00, 0x61, 0x00, 0x69, 0x00, 0x73, 0x00};
    unsigned char expected[] = {'f','r','a','n',231,'a','i','s'};

    unsigned char *out = NULL;
    size_t inl = 16;//strlen((char *)expected);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF16LE, inl, &out, BYTE_ISO_8859_1, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,8);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf16LeToUtf8_Setup(void **state){
    (void) state; //unused
    
    //français
    unsigned char in[] = {0x66, 0x00, 0x72, 0x00, 0x61, 0x00, 0x6e, 0x00, 0xe7, 0x00, 0x61, 0x00, 0x69, 0x00, 0x73, 0x00};
    unsigned char expected[] = "français";

    unsigned char *out = NULL;
    size_t inl = 16;//strlen((char *)expected);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF16LE, inl, &out, BYTE_UTF8, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);

    free(out);
}

static void byteConvertTextFormat_ConvertUtf16LeToUtf8V2_Setup(void **state){
    (void) state; //unused
    
    unsigned char in[] = {
     0xfe, 0x00, 0x65, 0x00, 0x74, 0x00, 0x74, 0x00, 0x61, 0x00, 0x20, 0x00, 
     0x65, 0x00, 0x72, 0x00, 0x75, 0x00, 0x20, 0x00, 0x6e, 0x00, 0x6f, 0x00, 
     0x6b, 0x00, 0x6b, 0x00, 0x75, 0x00, 0x72, 0x00, 0x20, 0x00, 0x6f, 0x00, 
     0x72, 0x00, 0xf0, 0x00, 0x20, 0x00, 0xe1, 0x00, 0x20, 0x00, 0xed, 0x00, 
     0x73, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x6e, 0x00, 0x73, 0x00, 0x6b, 0x00, 
     0x75, 0x00};
    unsigned char expected[] = "þetta eru nokkur orð á íslensku";

    unsigned char *out = NULL;
    size_t inl = 62;//strlen((char *)expected);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF16LE, inl, &out, BYTE_UTF8, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);
    free(out);
}

static void byteConvertTextFormat_ConvertUtf16LeToUtf16Be_Setup(void **state){
    (void) state; //unused
    
    unsigned char in[] = {
     0xfe, 0x00, 0x65, 0x00, 0x74, 0x00, 0x74, 0x00, 0x61, 0x00, 0x20, 0x00, 
     0x65, 0x00, 0x72, 0x00, 0x75, 0x00, 0x20, 0x00, 0x6e, 0x00, 0x6f, 0x00, 
     0x6b, 0x00, 0x6b, 0x00, 0x75, 0x00, 0x72, 0x00, 0x20, 0x00, 0x6f, 0x00, 
     0x72, 0x00, 0xf0, 0x00, 0x20, 0x00, 0xe1, 0x00, 0x20, 0x00, 0xed, 0x00, 
     0x73, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x6e, 0x00, 0x73, 0x00, 0x6b, 0x00, 
     0x75, 0x00};
    unsigned char expected[] = {
     0x00, 0xfe, 0x00, 0x65, 0x00, 0x74, 0x00, 0x74, 0x00, 0x61, 0x00, 0x20,
     0x00, 0x65, 0x00, 0x72, 0x00, 0x75, 0x00, 0x20, 0x00, 0x6e, 0x00, 0x6f, 
     0x00, 0x6b, 0x00, 0x6b, 0x00, 0x75, 0x00, 0x72, 0x00, 0x20, 0x00, 0x6f, 
     0x00, 0x72, 0x00, 0xf0, 0x00, 0x20, 0x00, 0xe1, 0x00, 0x20, 0x00, 0xed, 
     0x00, 0x73, 0x00, 0x6c, 0x00, 0x65, 0x00, 0x6e, 0x00, 0x73, 0x00, 0x6b, 
     0x00, 0x75};
    //þetta eru nokkur orð á íslensku

    unsigned char *out = NULL;
    size_t inl = 62;//strlen((char *)expected);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF16LE, inl, &out, BYTE_UTF16BE, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);
    free(out);
}

static void byteConvertTextFormat_ConvertUtf16LeToUtf16BeV2_Setup(void **state){
    (void) state; //unused
    
    unsigned char in[] = {
     0x28, 0x09, 0x47, 0x09, 0x2a, 0x09, 0x3e, 0x09, 0x32, 0x09, 0x20, 0x00,
     0x2c, 0x09, 0x3e, 0x09, 0x1f, 0x09, 0x20, 0x00, 0x28, 0x09, 0x2e, 0x09, 
     0x38, 0x09, 0x4d, 0x09, 0x15, 0x09, 0x3e, 0x09, 0x30, 0x09};
    unsigned char expected[] = {
     0x09, 0x28, 0x09, 0x47, 0x09, 0x2a, 0x09, 0x3e, 0x09, 0x32, 0x00, 0x20, 
     0x09, 0x2c, 0x09, 0x3e, 0x09, 0x1f, 0x00, 0x20, 0x09, 0x28, 0x09, 0x2e, 
     0x09, 0x38, 0x09, 0x4d, 0x09, 0x15, 0x09, 0x3e, 0x09, 0x30};
    //नेपाल बाट नमस्कार

    unsigned char *out = NULL;
    size_t inl = 34;//strlen((char *)expected);
    size_t outl = 0;
    
    bool v = byteConvertTextFormat(in, BYTE_UTF16LE, inl, &out, BYTE_UTF16BE, &outl);

    assert_true(v);
    assert_int_not_equal(outl,0);
    assert_memory_equal(expected,out,outl);
    free(out);
}

// for(int i = 0; i < outl; i++){
//     printf("[%x]",out[i]);
// }
// printf("\n");

int main(){
    
    const struct CMUnitTest tests[] = {
        //byteIsUtf8 tests
        cmocka_unit_test(byteIsUtf8_W3_Setup),
        cmocka_unit_test(byteIsUtf8_WithUtf16_Setup),
        //byteUtf16Strlen test
        cmocka_unit_test(byteUtf16Strlen_ForUtf16Le_Setup),
        cmocka_unit_test(byteUtf16Strlen_ForUtf16Be_Setup),
        cmocka_unit_test(byteUtf16Strlen_ForNull_Setup),
        //byteStrlen test
        cmocka_unit_test(byteStrlen_Latin1_Setup),
        cmocka_unit_test(byteStrlen_Ascii_Setup),
        cmocka_unit_test(byteStrlen_Utf8_Setup),
        cmocka_unit_test(byteStrlen_Utf16Le_Setup),
        cmocka_unit_test(byteStrlen_Utf16Be_Setup),
        cmocka_unit_test(byteStrlen_Unknown_Setup),
        //byteIsLegalEncoding test
        cmocka_unit_test(byteIsLegalEncoding_Latin1_Setup),
        cmocka_unit_test(byteIsLegalEncoding_Ascii_Setup),
        cmocka_unit_test(byteIsLegalEncoding_Utf8_Setup),
        cmocka_unit_test(byteIsLegalEncoding_Utf16Be_Setup),
        cmocka_unit_test(byteyteIsLegalEncoding_Utf16Le_Setup),
        cmocka_unit_test(byteIsLegalEncoding_NoEncoding_Setup),
        //byteHasBOM test
        cmocka_unit_test(byteHasBOM_DoesHaveBOM_Setup),
        cmocka_unit_test(byteHasBOM_DoesNotHaveBOM_Setup),
        //bytePrependBom test
        cmocka_unit_test(bytePrependBOM_FFFE_Setup),
        cmocka_unit_test(bytePrependBOM_FEFF_Setup),
        cmocka_unit_test(bytePrependBOM_Err_Setup),
        //byteConvertTextFormat
        //uses helper functions so they will not be tested on there own
        // - byteAsciiToUtf8
        // - byteUtf8ToAscii
        // - byteLat1ToUtf8
        // - byteUtf8ToLat1
        // - byteUtf16leToUtf8
        // - byteUtf8ToUtf16le
        // - byteUtf16beToUtf8
        // - byteUtf8ToUtf16be
        cmocka_unit_test(byteConvertTextFormat_ConvertErr_Setup),
        //ascii converts
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToLatin1_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToLatin1V2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToUtf16Le_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToUtf16LeV2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToUtf16Be_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToUtf16BeV2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToUtf8_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToUtf8V2_Setup),
        //latin1 converts
        cmocka_unit_test(byteConvertTextFormat_ConvertLatin1ToAscii_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertLatin1ToAsciiV2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertLatin1ToUtf16Le_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertLatin1ToUtf16LeV2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertLatin1ToUtf16Be_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertLatin1ToUtf16BeV2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertLatin1ToUtf8_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertLatin1ToUtf8V2_Setup),
        //utf8 converts
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf8ToAscii_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf8ToAsciiV2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf8ToLatin1_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf8ToLatin1V2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf8ToUtf16Le_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf8ToUtf16LeV2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf8ToUtf16Be_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf8ToUtf16BeV2_Setup),
        //utf16 be converts
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16BeToAscii_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16BeToAsciiV2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16BeToLatin1_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16BeToLatin1V2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16BeToUtf8_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16BeToUtf8V2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16BeToUtf16Le_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16BeToUtf16LeV2_Setup),
        //utf16 le converts
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16LeToAscii_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16LeToAsciiV2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16LeToLatin1_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16LeToLatin1V2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16LeToUtf8_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16LeToUtf8V2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16LeToUtf16Be_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertUtf16LeToUtf16BeV2_Setup),    
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}