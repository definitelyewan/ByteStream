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

static void byteUnicode_IsUtf8W3Setup(void **state){
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

static void byteUnicode_IsUtf8WithUtf16Setup(void **state){
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

static void byteUnicode_Utf16LenForUtf16LeSetup(void **state){
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

static void byteUnicode_Utf16LenForUtf16BeSetup(void **state){
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

static void byteUnicode_Utf16LenForNullSetup(void **state){
    (void) state; //unused
    size_t s = 0;

    s = byteUtf16Strlen(NULL);
    assert_int_equal(s,0);

}

static void byteUnicode_ByteStrlenLatin1Setup(void **state){
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

static void byteUnicode_ByteStrlenAsciiSetup(void **state){
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

static void byteUnicode_ByteStrlenUtf8Setup(void **state){
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

static void byteUnicode_ByteStrlenUtf16LeSetup(void **state){
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

static void byteUnicode_ByteStrlenUtf16BeSetup(void **state){
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

static void byteUnicode_ByteStrlenUnknownSetup(void **state){
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

static void byteUnicode_ByteIsLegalLatin1Setup(void **state){
    (void) state; //unused
    
    bool v = byteIsLegalEncoding(BYTE_ISO_8859_1);
    assert_true(v);
}

static void byteUnicode_ByteIsLegalAsciiSetup(void **state){
    (void) state; //unused
    
    bool v = byteIsLegalEncoding(BYTE_ASCII);
    assert_true(v);
}

static void byteUnicode_ByteIsLegalUtf8Setup(void **state){
    (void) state; //unused
    
    bool v = byteIsLegalEncoding(BYTE_UTF8);
    assert_true(v);
}

static void byteUnicode_ByteIsLegalUtf16BeSetup(void **state){
    (void) state; //unused
    
    bool v = byteIsLegalEncoding(BYTE_UTF16BE);
    assert_true(v);
}

static void byteUnicode_ByteIsLegalUtf16LeSetup(void **state){
    (void) state; //unused
    
    bool v = byteIsLegalEncoding(BYTE_UTF16LE);
    assert_true(v);
}

static void byteUnicode_ByteIsLegalNoEncodingSetup(void **state){
    (void) state; //unused
    
    bool v = byteIsLegalEncoding(9);
    assert_false(v);
}

static void byteUnicode_DoesHaveBOMSetup(void **state){
    (void) state; //unused

    unsigned char a[2] = {0xff,0xfe};
    unsigned char b[2] = {0xfe,0xff};

    assert_true(byteHasBOM(a));
    assert_true(byteHasBOM(b));
}

static void byteUnicode_DoesNotHaveBOMSetup(void **state){
    (void) state; //unused

    unsigned char a[2] = {0x00,0x00};

    assert_false(byteHasBOM(a));
}

static void byteUnicode_PrependBOMFFFESetup(void **state){
    (void) state; //unused

    unsigned char *pass = calloc(1,8);
    strcpy((char *)pass, "bomfffe");

    size_t s = strlen((char *)pass);
    size_t oldS = s;
    bool v = bytePrependBOM(&pass, &s);

    assert_true(v);
    assert_int_not_equal(oldS, s);
    assert_memory_equal("\xff\xfe",pass,2);
    free(pass);
}

static void byteUnicode_PrependBOMFEFFSetup(void **state){
    (void) state; //unused

    unsigned char *pass = calloc(1,8);
    strcpy((char *)pass, "bomfeff");

    size_t s = strlen((char *)pass);
    size_t oldS = s;
    pass[0] = '\0';

    bool v = bytePrependBOM(&pass, &s);

    assert_true(v);
    assert_int_not_equal(oldS, s);
    assert_memory_equal("\xfe\xff",pass,2);
    free(pass);
}

static void byteUnicode_PrependBOMErrSetup(void **state){
    (void) state; //unused

    size_t s = 1;
    bool v = bytePrependBOM(NULL, &s);

    assert_false(v);
}

int main(){
    
    const struct CMUnitTest tests[] = {
        //byteIsUtf8 tests
        cmocka_unit_test(byteUnicode_IsUtf8W3Setup),
        cmocka_unit_test(byteUnicode_IsUtf8WithUtf16Setup),
        //byteUtf16Strlen test
        cmocka_unit_test(byteUnicode_Utf16LenForUtf16LeSetup),
        cmocka_unit_test(byteUnicode_Utf16LenForUtf16BeSetup),
        cmocka_unit_test(byteUnicode_Utf16LenForNullSetup),
        //byteStrlen test
        cmocka_unit_test(byteUnicode_ByteStrlenLatin1Setup),
        cmocka_unit_test(byteUnicode_ByteStrlenAsciiSetup),
        cmocka_unit_test(byteUnicode_ByteStrlenUtf8Setup),
        cmocka_unit_test(byteUnicode_ByteStrlenUtf16LeSetup),
        cmocka_unit_test(byteUnicode_ByteStrlenUtf16BeSetup),
        cmocka_unit_test(byteUnicode_ByteStrlenUnknownSetup),
        //byteIsLegalEncoding test
        cmocka_unit_test(byteUnicode_ByteIsLegalLatin1Setup),
        cmocka_unit_test(byteUnicode_ByteIsLegalAsciiSetup),
        cmocka_unit_test(byteUnicode_ByteIsLegalUtf8Setup),
        cmocka_unit_test(byteUnicode_ByteIsLegalUtf16BeSetup),
        cmocka_unit_test(byteUnicode_ByteIsLegalUtf16LeSetup),
        cmocka_unit_test(byteUnicode_ByteIsLegalNoEncodingSetup),
        //byteHasBOM test
        cmocka_unit_test(byteUnicode_DoesHaveBOMSetup),
        cmocka_unit_test(byteUnicode_DoesNotHaveBOMSetup),
        //bytePrependBom test
        cmocka_unit_test(byteUnicode_PrependBOMFFFESetup),
        cmocka_unit_test(byteUnicode_PrependBOMFEFFSetup),
        cmocka_unit_test(byteUnicode_PrependBOMErrSetup),
        
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}