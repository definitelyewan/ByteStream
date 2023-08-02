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
    bool v = bytePrependBOM(&pass, &s);

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

    bool v = bytePrependBOM(&pass, &s);

    assert_true(v);
    assert_int_not_equal(oldS, s);
    assert_memory_equal("\xfe\xff",pass,2);
    free(pass);
}

static void bytePrependBOM_Err_Setup(void **state){
    (void) state; //unused

    size_t s = 1;
    bool v = bytePrependBOM(NULL, &s);

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
}

static void byteConvertTextFormat_ConvertAsciiToUtf16Le_Setup(void **state){
    (void) state; //unused

    unsigned char in[] = "ascii to utf16le test";
    unsigned char expected[] = {'a', 0, 's', 0, 'c', 0, 'i', 0, 'i', 0, ' ', 
     0, 't', 0, 'o', 0, ' ', 0, 'u', 0, 't', 0, 'f', 0, '1', 0, '6', 0, 'l', 
     0, 'e', 0, ' ', 0, 't', 0, 'e', 0, 's', 0, 't', 0};

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
    unsigned char expected[] = {'H', 0, 'e', 0, 'l', 0, 'l', 0, 'o', 0, ',',
     0, ' ', 0, 'W', 0, 'o', 0, 'r', 0, 'l', 0, 'd', 0, '!', 0};

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
    unsigned char expected[] = {0, 'a', 0, 's', 0, 'c', 0, 'i', 0, 'i', 0, ' ', 
     0, 't', 0, 'o', 0, ' ', 0, 'u', 0, 't', 0, 'f', 0, '1', 0, '6', 0, 'l', 
     0, 'e', 0, ' ', 0, 't', 0, 'e', 0, 's', 0, 't'};

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
    unsigned char expected[] = {0, 'H', 0, 'e', 0, 'l', 0, 'l', 0, 'o', 0, ',',
     0, ' ', 0, 'W', 0, 'o', 0, 'r', 0, 'l', 0, 'd', 0, '!'};

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
        //ascii converts
        cmocka_unit_test(byteConvertTextFormat_ConvertErr_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToLatin1_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToLatin1V2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToUtf16Le_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToUtf16LeV2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToUtf16Be_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToUtf16BeV2_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToUtf8_Setup),
        cmocka_unit_test(byteConvertTextFormat_ConvertAsciiToUtf8V2_Setup),
        //latin1 converts
        
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}