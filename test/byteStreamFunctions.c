#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include <byteStream.h>

static void byteStreamCreate_NormalSetup(void **state){
    (void) state; /* unused */

    ByteStream *stream = NULL;

    unsigned char b[] = "this is a buffer the stream";
    size_t s = strlen((char *)b);
    stream = byteStreamCreate(b, strlen((char *)b));

    assert_non_null(stream);
    assert_non_null(stream->buffer);
    assert_memory_equal(b, "this is a buffer the stream", s);
    assert_int_equal(s, strlen((char *)stream->buffer));


    free(stream->buffer);
    free(stream);

}

static void byteStreamCreate_NormalSetup2(void **state){
    (void) state; /* unused */

    ByteStream *stream = NULL;
    size_t s = 2;

    stream = byteStreamCreate(NULL, 2);

    assert_non_null(stream);
    assert_non_null(stream->buffer);
    assert_int_equal(s, stream->bufferSize);


    free(stream->buffer);
    free(stream);

}

static void byteStreamCreate_SmallSizeSetup(void **state){
    (void) state; /* unused */

    ByteStream *stream = NULL;

    unsigned char b[] = "this is a buffer the stream";
    int s = strlen((char *)b) / 2;
    int bytesStored = 0;
    stream = byteStreamCreate(b, s);

    assert_non_null(stream);
    assert_non_null(stream->buffer);

    for(bytesStored = 0; stream->buffer[bytesStored] != 0; bytesStored++);
    assert_int_equal(s, bytesStored);

    assert_memory_equal(b, "this is a buffer the stream", s);

    free(stream->buffer);
    free(stream);
}

static void byteStreamCreate_NoSizeSetup(void **state){
    (void) state; /* unused */

    ByteStream *stream = NULL;

    unsigned char b[] = "this is a buffer the stream";

    stream = byteStreamCreate(b, 0);

    assert_null(stream);
}

static void byteStreamFree_NormalSetup(void **state){
    (void) state; /* unused */

    ByteStream *stream = byteStreamCreate((unsigned char *)"test function", 14);

    byteStreamFree(stream);

    assert_int_equal(stream->cursor, 0);
    assert_int_equal(stream->bufferSize, 0);
    assert_null(stream->buffer);

    free(stream);
}

static void byteStreamFree_NoBufferFreeSetup(void **state){
    (void) state; /* unused */

    ByteStream *stream = byteStreamCreate(NULL, 15);

    free(stream->buffer);
    stream->buffer = NULL;

    byteStreamFree(stream);
    assert_int_equal(stream->cursor, 0);
    assert_int_equal(stream->bufferSize, 0);

    byteStreamDestroy(stream);
}

static void byteStreamResize_IncreaseSetup(void **state){
    (void) state; /* unused */

    ByteStream *stream = byteStreamCreate(NULL, 100);

    byteStreamResize(stream, 120);
    
    assert_non_null(stream->buffer);
    assert_int_equal(stream->bufferSize, 120);

    byteStreamDestroy(stream);
    
}

static void byteStreamResize_DecreaseSetup(void **state){
    (void) state; /* unused */

    ByteStream *stream = byteStreamCreate(NULL, 100);

    byteStreamResize(stream, 50);
    
    assert_non_null(stream->buffer);
    assert_int_equal(stream->bufferSize, 50);
    
    byteStreamDestroy(stream);
}

static void byteStreamResize_FailSetup(void **state){
    (void) state; /* unused */

    ByteStream *stream = byteStreamCreate(NULL, 100);

    byteStreamResize(stream, 0);
    
    assert_non_null(stream->buffer);
    assert_int_equal(stream->bufferSize, 100);
    
    byteStreamDestroy(stream);
}

static void byteStream_ReadNothingSetup(void **state){
    (void) state; /* unused */


    unsigned char b[] = "this is a binary block?";
    unsigned char read[10];

    size_t s = strlen((char *)b);
    ByteStream *stream = byteStreamCreate(b,s);

    bool v = byteStreamRead(stream, read, 0);

    assert_false(v);

    byteStreamDestroy(stream);
}

static void byteStream_ReadTooMuchSetup(void **state){
    (void) state; /* unused */


    unsigned char b[] = "this is a binary block?";
    unsigned char read[50];

    size_t s = strlen((char *)b);
    ByteStream *stream = byteStreamCreate(b,s);


    bool v = byteStreamRead(stream, read, 50);

    assert_memory_equal(b,read,s);
    assert_int_equal(s, stream->cursor);
    assert_true(v);

    byteStreamDestroy(stream);
}

static void byteStream_ReadWithMaxCursorSetup(void **state){
    (void) state; /* unused */


    unsigned char b[] = "this is a binary block?";
    unsigned char read[50] = {0};

    size_t s = strlen((char *)b);
    ByteStream *stream = byteStreamCreate(b,s);
    stream->cursor = s;

    bool v = byteStreamRead(stream, read, 12);

    assert_memory_not_equal(b,read,s);
    assert_false(v);

    byteStreamDestroy(stream);
}

static void byteStream_ReadHalfBlockSetup(void **state){
    (void) state; /* unused */

    unsigned char b[] = "this is a binary block?";
    unsigned char read[50];

    size_t s = strlen((char *)b);
    ByteStream *stream = byteStreamCreate(b,s);

    bool v = byteStreamRead(stream, read, s/2);

    assert_memory_equal("this is a bin",read,s/2);
    assert_true(v);

    byteStreamDestroy(stream);
}

static void byteStream_NormalReadSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "this is a binary block?";
    unsigned char read[50];

    ByteStream *stream = byteStreamCreate(b,strlen((char *)b));

    bool v = byteStreamRead(stream, read, 4);

    assert_memory_equal(b,read,4);
    assert_true(v);

    byteStreamDestroy(stream);
}

static void byteStream_SeekSetSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "seek testing";

    ByteStream *stream = byteStreamCreate(b,strlen((char *)b));

    bool v = byteStreamSeek(stream, 4, SEEK_SET);

    assert_int_equal(stream->cursor, 4);
    assert_true(v);

    byteStreamDestroy(stream);
}

static void byteStream_SeekSetBigSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "seek testing";

    ByteStream *stream = byteStreamCreate(b,strlen((char *)b));

    bool v = byteStreamSeek(stream, 900000, SEEK_SET);

    assert_int_equal(stream->cursor, 0);
    assert_false(v);

    byteStreamDestroy(stream);
}

static void byteStream_SeekCurSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "seek testing";

    ByteStream *stream = byteStreamCreate(b,strlen((char *)b));
    stream->cursor = 5;

    bool v = byteStreamSeek(stream, 3, SEEK_CUR);

    assert_int_equal(stream->cursor, 8);
    assert_true(v);

    byteStreamDestroy(stream);
}

static void byteStream_SeekCurBigSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "seek testing";

    ByteStream *stream = byteStreamCreate(b,strlen((char *)b));
    stream->cursor = 5;

    bool v = byteStreamSeek(stream, 10, SEEK_CUR);

    assert_int_not_equal(stream->cursor, 15);
    assert_false(v);

    byteStreamDestroy(stream);
}

static void byteStream_SeekEndBigSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "seek testing";

    ByteStream *stream = byteStreamCreate(b,strlen((char *)b));

    bool v = byteStreamSeek(stream, 9000, SEEK_CUR);

    assert_int_equal(stream->cursor, 0);
    assert_false(v);

    byteStreamDestroy(stream);
}

static void byteStream_SeekEndSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "seek testing";

    ByteStream *stream = byteStreamCreate(b,strlen((char *)b));

    bool v = byteStreamSeek(stream, 0, SEEK_END);

    assert_int_equal(stream->cursor, 12);
    assert_true(v);

    byteStreamDestroy(stream);
}

static void byteStream_ReadUntilCharacterSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "read me!!! you wont get passed this * how?";
    unsigned char *read = NULL;
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    read = byteStreamReadUntil(stream,'*');

    assert_non_null(read);
    assert_memory_equal(read,"read me!!! you wont get passed this *",37);

    if(read){
        free(read);
    }

    byteStreamDestroy(stream);
}

static void byteStream_ReadUntilCharacterNotFoundSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "read me!!! you wont get passed this * how?";
    unsigned char *read = NULL;
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    read = byteStreamReadUntil(stream,')');

    assert_null(read);

    byteStreamDestroy(stream);
}

static void byteStream_SearchAndReplaceSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "and anD aNd Notand And";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    byteStreamSearchAndReplace(stream, (unsigned char *)"and", 3, (unsigned char *)"???", 3);

    assert_memory_equal("???",(stream->buffer) + 0, 3);
    assert_memory_equal("???",(stream->buffer) + 15, 3);

    byteStreamDestroy(stream);
}

static void byteStream_SearchAndReplaceNotFoundSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "and anD aNd Notand And";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    byteStreamSearchAndReplace(stream, (unsigned char *)"what", 4, (unsigned char *)"???", 3);

    assert_memory_equal(b,stream->buffer, s);

    byteStreamDestroy(stream);
}

static void byteStream_SearchAndReplaceShortPatternSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "and anD aNd Notand And";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    byteStreamSearchAndReplace(stream, (unsigned char *)"an", 2, (unsigned char *)"???", 3);

    assert_memory_equal("??",(stream->buffer) + 0, 2);
    assert_memory_not_equal("???",(stream->buffer) + 0, 3);

    assert_memory_equal("??",(stream->buffer) + 4, 2);
    assert_memory_not_equal("???",(stream->buffer) + 4, 3);

    assert_memory_equal("??",(stream->buffer) + 15, 2);
    assert_memory_not_equal("???",(stream->buffer) + 15, 3);

    byteStreamDestroy(stream);
}

static void byteStream_SearchAndReplaceShortReplaceSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "and anD aNd Notand And";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    byteStreamSearchAndReplace(stream, (unsigned char *)"Not", 3, (unsigned char *)"x", 1);
    
    assert_memory_not_equal("x",(stream->buffer) + 13, 1);

    byteStreamDestroy(stream);
}

static void byteStream_CursorSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    assert_int_equal(byteStreamCursor(stream)[0],'0');

    stream->cursor = 5;

    assert_int_equal(byteStreamCursor(stream)[0],'5');

    byteStreamDestroy(stream);
}

static void byteStream_GetChSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    assert_int_equal(byteStreamGetCh(stream),'0');

    stream->cursor = 5;

    assert_int_equal(byteStreamGetCh(stream),'5');

    byteStreamDestroy(stream);
}

static void byteStream_GetChEOFSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    stream->cursor = 90;

    assert_int_equal(byteStreamGetCh(stream),EOF);

    byteStreamDestroy(stream);
}

static void byteStream_WriteSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    bool v = byteStreamWrite(stream, (unsigned char *)"xxxx",4);

    assert_true(v);
    assert_memory_equal(stream->buffer, "xxxx", 4);

    byteStreamDestroy(stream);
}

static void byteStream_WriteMidSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);
    stream->cursor = 5;
    bool v = byteStreamWrite(stream, (unsigned char *)"xxxx",4);

    assert_true(v);
    assert_memory_equal((stream->buffer) + 5, "xxxx", 4);

    byteStreamDestroy(stream);
}

static void byteStream_WriteEndSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);
    stream->cursor = 9;
    bool v = byteStreamWrite(stream, (unsigned char *)"xxxx",4);

    assert_true(v);
    assert_memory_equal((stream->buffer) + 9, "x", 1);

    byteStreamDestroy(stream);
}

static void byteStream_WriteErrSetup(void **state){
    (void) state; //unused

    bool v = byteStreamWrite(NULL, NULL, 9999);

    assert_false(v);
}

static void byteStream_WriteAtPositionErrSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);
    bool v = byteStreamWriteAtPosition(stream, (unsigned char *)"xxxx",4,9);

    assert_true(v);
    assert_memory_equal((stream->buffer) + 9, "x", 1);

    byteStreamDestroy(stream);
}

static void byteStream_WriteAtPositionStartSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);
    bool v = byteStreamWriteAtPosition(stream, (unsigned char *)"xxxx",4,0);

    assert_true(v);
    assert_memory_equal((stream->buffer) + 0, "xxxx", 4);

    byteStreamDestroy(stream);
}

static void byteStream_WriteAtPositionMidSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);
    bool v = byteStreamWriteAtPosition(stream, (unsigned char *)"xxxx",4,5);

    assert_true(v);
    assert_memory_equal((stream->buffer) + 5, "xxxx", 4);

    byteStreamDestroy(stream);

}

static void byteStream_ReturnAsciiWordSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "ascii\0word\0test";
    unsigned char *read = NULL;
    size_t s = 16;
    size_t out = 0;

    ByteStream *stream = byteStreamCreate(b, s);

    read = byteStreamReturnAscii(stream, &out);
    assert_int_not_equal(out, 0);
    assert_string_equal("ascii",(char *)read);
    free(read);

    read = byteStreamReturnAscii(stream, &out);
    assert_int_not_equal(out, 0);
    assert_string_equal("word",(char *)read);
    free(read);

    read = byteStreamReturnAscii(stream, &out);
    assert_int_not_equal(out, 0);
    assert_string_equal("test",(char *)read);
    free(read);

    byteStreamDestroy(stream);
}

static void byteStream_ReturnAsciiFullBufferSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "ascii word test";
    unsigned char *read = NULL;
    size_t s = 16;
    size_t out = 0;

    ByteStream *stream = byteStreamCreate(b, s);

    read = byteStreamReturnAscii(stream, &out);
    assert_int_not_equal(out, 0);
    assert_string_equal("ascii word test",(char *)read);
    free(read);

    byteStreamDestroy(stream);
}

static void byteStream_ReturnAsciiFailOnDoubleNullSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "ascii\0\0test";
    unsigned char *read = NULL;
    size_t s = 16;
    size_t out = 0;

    ByteStream *stream = byteStreamCreate(b, s);

    read = byteStreamReturnAscii(stream, &out);
    assert_int_not_equal(out, 0);
    assert_string_equal("ascii",(char *)read);
    free(read);

    read = byteStreamReturnAscii(stream, &out);
    assert_int_equal(out, 0);
    assert_null(read);
    free(read);

    byteStreamDestroy(stream);
}

static void byteStream_ReturnAsciiFailWithNoBytesSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "\0\0";
    unsigned char *read = NULL;
    size_t s = 3;
    size_t out = 0;

    ByteStream *stream = byteStreamCreate(b, s);

    read = byteStreamReturnAscii(stream, &out);
    assert_int_equal(out, 0);
    assert_null(read);
    free(read);

    byteStreamDestroy(stream);
}

static void byteStream_ReturnUtf16WordSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "\0u\0t\0f\0\0w\0o\0r\0d\0\0t\0e\0s\0t\0\0";
    unsigned char *read = NULL;
    size_t s = 30;
    size_t out = 0;

    ByteStream *stream = byteStreamCreate(b, s);

    read = byteStreamReturnUtf16(stream, &out);
    assert_int_not_equal(out, 0);
    assert_memory_equal("\0u\0t\0f\0",read,out);
    free(read);

    read = byteStreamReturnUtf16(stream, &out);
    assert_int_not_equal(out, 0);
    assert_memory_equal("\0w\0o\0r\0d\0",read,out);
    free(read); 

    read = byteStreamReturnUtf16(stream, &out);
    assert_int_not_equal(out, 0);
    assert_memory_equal("\0t\0e\0s\0t\0",read,out);
    free(read); 

    byteStreamDestroy(stream);
}

static void byteStream_ReturnUtf16FullBufferSetup(void **state){
    (void) state; //unused

    unsigned char b[] = "u\0t\0f\0 \0w\0o\0r\0d\0 \0t\0e\0s\0t\0\0";
    unsigned char *read = NULL;
    size_t s = 30;
    size_t out = 0;

    ByteStream *stream = byteStreamCreate(b, s);

    read = byteStreamReturnUtf16(stream, &out);
    assert_int_not_equal(out, 0);
    assert_memory_equal(b,read,out);
    free(read);

    byteStreamDestroy(stream);
}

int main(){
    
    const struct CMUnitTest tests[] = {
        //byteStreamCreate tests
        cmocka_unit_test(byteStreamCreate_NormalSetup),
        cmocka_unit_test(byteStreamCreate_NormalSetup2),
        cmocka_unit_test(byteStreamCreate_SmallSizeSetup),
        cmocka_unit_test(byteStreamCreate_NoSizeSetup),
        //byteStreamFree tests
        cmocka_unit_test(byteStreamFree_NormalSetup),
        cmocka_unit_test(byteStreamFree_NoBufferFreeSetup),
        //byteStreamResize tests
        cmocka_unit_test(byteStreamResize_IncreaseSetup),
        cmocka_unit_test(byteStreamResize_DecreaseSetup),
        cmocka_unit_test(byteStreamResize_FailSetup),
        //byteStreamRead tests
        cmocka_unit_test(byteStream_ReadNothingSetup),
        cmocka_unit_test(byteStream_ReadTooMuchSetup),
        cmocka_unit_test(byteStream_ReadWithMaxCursorSetup),
        cmocka_unit_test(byteStream_ReadHalfBlockSetup),
        cmocka_unit_test(byteStream_NormalReadSetup),
        //byteStreamSeek tests
        cmocka_unit_test(byteStream_SeekSetSetup),
        cmocka_unit_test(byteStream_SeekSetBigSetup),
        cmocka_unit_test(byteStream_SeekCurSetup),
        cmocka_unit_test(byteStream_SeekCurBigSetup),
        cmocka_unit_test(byteStream_SeekEndBigSetup),
        cmocka_unit_test(byteStream_SeekEndSetup),
        //byteStreamReadUntil test
        cmocka_unit_test(byteStream_ReadUntilCharacterSetup),
        cmocka_unit_test(byteStream_ReadUntilCharacterNotFoundSetup),
        //byteStreamSearchAndReplace test
        cmocka_unit_test(byteStream_SearchAndReplaceSetup),
        cmocka_unit_test(byteStream_SearchAndReplaceNotFoundSetup),
        cmocka_unit_test(byteStream_SearchAndReplaceShortPatternSetup),
        cmocka_unit_test(byteStream_SearchAndReplaceShortReplaceSetup),
        //byteStreamCursor test
        cmocka_unit_test(byteStream_CursorSetup),
        //byteStreamGetCh test
        cmocka_unit_test(byteStream_GetChSetup),
        cmocka_unit_test(byteStream_GetChEOFSetup),
        //byteStreamWrite
        cmocka_unit_test(byteStream_WriteSetup),
        cmocka_unit_test(byteStream_WriteMidSetup),
        cmocka_unit_test(byteStream_WriteEndSetup),
        cmocka_unit_test(byteStream_WriteErrSetup),
        //byteStreamWriteAtPosition
        cmocka_unit_test(byteStream_WriteAtPositionErrSetup),
        cmocka_unit_test(byteStream_WriteAtPositionStartSetup),
        cmocka_unit_test(byteStream_WriteAtPositionMidSetup),
        //byteStreamReturnAscii
        //same function is called for:
        // - byteStreamReturnLatin1
        // - byteStreamReturnUtf8
        cmocka_unit_test(byteStream_ReturnAsciiWordSetup),
        cmocka_unit_test(byteStream_ReturnAsciiFullBufferSetup),
        cmocka_unit_test(byteStream_ReturnAsciiFailOnDoubleNullSetup),
        cmocka_unit_test(byteStream_ReturnAsciiFailWithNoBytesSetup),
        //byteStreamReturnUtf16
        cmocka_unit_test(byteStream_ReturnUtf16WordSetup),
        cmocka_unit_test(byteStream_ReturnUtf16FullBufferSetup),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}