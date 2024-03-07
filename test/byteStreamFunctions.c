#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include <byteStream.h>

static void byteStreamFromFile_Normal_Setup(void **state){
    (void) state; /* unused */

    ByteStream *stream = NULL;

    stream = byteStreamFromFile("assets/latin1.txt");

    assert_non_null(stream->buffer);
    byteStreamDestroy(stream);
}

static void byteStreamFromFile_Fail_Setup(void **state){
    (void) state; /* unused */

    ByteStream *stream = NULL;

    stream = byteStreamFromFile("assets/latin1.sdsdtxt");

    assert_null(stream);
}

static void byteStreamCreate_Normal_Setup(void **state){
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

static void byteStreamCreate_Normal2_Setup(void **state){
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

static void byteStreamCreate_SmallSize_Setup(void **state){
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

static void byteStreamCreate_NoSize_Setup(void **state){
    (void) state; /* unused */

    ByteStream *stream = NULL;

    unsigned char b[] = "this is a buffer the stream";

    stream = byteStreamCreate(b, 0);

    assert_null(stream);
}

static void byteStreamFree_Normal_Setup(void **state){
    (void) state; /* unused */

    ByteStream *stream = byteStreamCreate((unsigned char *)"test function", 14);

    byteStreamFree(stream);

    assert_int_equal(stream->cursor, 0);
    assert_int_equal(stream->bufferSize, 0);
    assert_null(stream->buffer);

    free(stream);
}

static void byteStreamFree_NoBufferFree_Setup(void **state){
    (void) state; /* unused */

    ByteStream *stream = byteStreamCreate(NULL, 15);

    free(stream->buffer);
    stream->buffer = NULL;

    byteStreamFree(stream);
    assert_int_equal(stream->cursor, 0);
    assert_int_equal(stream->bufferSize, 0);

    byteStreamDestroy(stream);
}

static void byteStreamResize_Increase_Setup(void **state){
    (void) state; /* unused */

    ByteStream *stream = byteStreamCreate(NULL, 100);

    byteStreamResize(stream, 120);
    
    assert_non_null(stream->buffer);
    assert_int_equal(stream->bufferSize, 120);

    byteStreamDestroy(stream);
    
}

static void byteStreamResize_Decrease_Setup(void **state){
    (void) state; /* unused */

    ByteStream *stream = byteStreamCreate(NULL, 100);

    byteStreamResize(stream, 50);
    
    assert_non_null(stream->buffer);
    assert_int_equal(stream->bufferSize, 50);
    
    byteStreamDestroy(stream);
}

static void byteStreamResize_Fail_Setup(void **state){
    (void) state; /* unused */

    ByteStream *stream = byteStreamCreate(NULL, 100);

    byteStreamResize(stream, 0);
    
    assert_non_null(stream->buffer);
    assert_int_equal(stream->bufferSize, 100);
    
    byteStreamDestroy(stream);
}

static void byteStreamRead_ReadNothing_Setup(void **state){
    (void) state; /* unused */


    unsigned char b[] = "this is a binary block?";
    unsigned char read[10];

    size_t s = strlen((char *)b);
    ByteStream *stream = byteStreamCreate(b,s);

    bool v = byteStreamRead(stream, read, 0);

    assert_false(v);

    byteStreamDestroy(stream);
}

static void byteStreamRead_ReadTooMuch_Setup(void **state){
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

static void byteStreamRead_ReadWithMaxCursor_Setup(void **state){
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

static void byteStreamRead_HalfBlock_Setup(void **state){
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

static void byteStreamRead_Normal_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "this is a binary block?";
    unsigned char read[50];

    ByteStream *stream = byteStreamCreate(b,strlen((char *)b));

    bool v = byteStreamRead(stream, read, 4);

    assert_memory_equal(b,read,4);
    assert_true(v);

    byteStreamDestroy(stream);
}

static void byteStreamSeek_Set_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "seek testing";

    ByteStream *stream = byteStreamCreate(b,strlen((char *)b));

    bool v = byteStreamSeek(stream, 4, SEEK_SET);

    assert_int_equal(stream->cursor, 4);
    assert_true(v);

    byteStreamDestroy(stream);
}

static void byteStreamSeek_SetBig_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "seek testing";

    ByteStream *stream = byteStreamCreate(b,strlen((char *)b));

    bool v = byteStreamSeek(stream, 900000, SEEK_SET);

    assert_int_equal(stream->cursor, 0);
    assert_false(v);

    byteStreamDestroy(stream);
}

static void byteStreamSeek_Cur_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "seek testing";

    ByteStream *stream = byteStreamCreate(b,strlen((char *)b));
    stream->cursor = 5;

    bool v = byteStreamSeek(stream, 3, SEEK_CUR);

    assert_int_equal(stream->cursor, 8);
    assert_true(v);

    byteStreamDestroy(stream);
}

static void byteStreamSeek_CurBig_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "seek testing";

    ByteStream *stream = byteStreamCreate(b,strlen((char *)b));
    stream->cursor = 5;

    bool v = byteStreamSeek(stream, 10, SEEK_CUR);

    assert_int_not_equal(stream->cursor, 15);
    assert_false(v);

    byteStreamDestroy(stream);
}

static void byteStreamSeek_EndBig_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "seek testing";

    ByteStream *stream = byteStreamCreate(b,strlen((char *)b));

    bool v = byteStreamSeek(stream, 9000, SEEK_CUR);

    assert_int_equal(stream->cursor, 0);
    assert_false(v);

    byteStreamDestroy(stream);
}

static void byteStreamSeek_End_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "seek testing";

    ByteStream *stream = byteStreamCreate(b,strlen((char *)b));

    bool v = byteStreamSeek(stream, 0, SEEK_END);

    assert_int_equal(stream->cursor, 12);
    assert_true(v);

    byteStreamDestroy(stream);
}

static void byteStreamReadUntil_SpecificCharacter_Setup(void **state){
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

static void byteStreamReadUntil_CharacterNotFound_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "read me!!! you wont get passed this * how?";
    unsigned char *read = NULL;
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    read = byteStreamReadUntil(stream,')');

    assert_null(read);

    byteStreamDestroy(stream);
}

static void byteStreamSearchAndReplace_SearchAndReplace_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "and anD aNd Notand And";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    byteStreamSearchAndReplace(stream, (unsigned char *)"and", 3, (unsigned char *)"???", 3);    
    assert_memory_equal("???",(stream->buffer) + 0, 3);
    assert_memory_equal("???",(stream->buffer) + 15, 3);

    byteStreamDestroy(stream);
}

static void byteStreamSearchAndReplace_NotFound_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "and anD aNd Notand And";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    byteStreamSearchAndReplace(stream, (unsigned char *)"what", 4, (unsigned char *)"???", 3);

    assert_memory_equal(b,stream->buffer, s);

    byteStreamDestroy(stream);
}

static void byteStreamSearchAndReplace_ShortPattern_Setup(void **state){
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

static void byteStreamSearchAndReplace_ShortReplace_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "and anD aNd Notand And";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    byteStreamSearchAndReplace(stream, (unsigned char *)"Not", 3, (unsigned char *)"x", 1);
    
    assert_memory_not_equal("x",(stream->buffer) + 13, 1);

    byteStreamDestroy(stream);
}

static void byteStreamCursor_Normal_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    assert_int_equal(byteStreamCursor(stream)[0],'0');

    stream->cursor = 5;

    assert_int_equal(byteStreamCursor(stream)[0],'5');

    byteStreamDestroy(stream);
}

static void byteStreamGetCh_Normal_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    assert_int_equal(byteStreamGetCh(stream),'0');

    stream->cursor = 5;

    assert_int_equal(byteStreamGetCh(stream),'5');

    byteStreamDestroy(stream);
}

static void byteStreamGetCh_EOF_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    stream->cursor = 90;

    assert_int_equal(byteStreamGetCh(stream),EOF);

    byteStreamDestroy(stream);
}

static void byteStreamWrite_Normal_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);

    bool v = byteStreamWrite(stream, (unsigned char *)"xxxx",4);

    assert_true(v);
    assert_memory_equal(stream->buffer, "xxxx", 4);

    byteStreamDestroy(stream);
}

static void byteStreamWrite_AtMid_Setup(void **state){
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

static void byteStreamWrite_AtEnd_Setup(void **state){
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

static void byteStreamWrite_Err_Setup(void **state){
    (void) state; //unused

    bool v = byteStreamWrite(NULL, NULL, 9999);

    assert_false(v);
}

static void byteStreamWriteAtPosition_Err_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);
    bool v = byteStreamWriteAtPosition(stream, (unsigned char *)"xxxx",4,9);

    assert_true(v);
    assert_memory_equal((stream->buffer) + 9, "x", 1);

    byteStreamDestroy(stream);
}

static void byteStreamWriteAtPosition_Start_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);
    bool v = byteStreamWriteAtPosition(stream, (unsigned char *)"xxxx",4,0);

    assert_true(v);
    assert_memory_equal((stream->buffer) + 0, "xxxx", 4);

    byteStreamDestroy(stream);
}

static void byteStreamWriteAtPosition_Mid_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "0123456789";
    size_t s = strlen((char *)b);
    
    ByteStream *stream = byteStreamCreate(b, s);
    bool v = byteStreamWriteAtPosition(stream, (unsigned char *)"xxxx",4,5);

    assert_true(v);
    assert_memory_equal((stream->buffer) + 5, "xxxx", 4);

    byteStreamDestroy(stream);

}

static void byteStreamReturnAscii_Word_Setup(void **state){
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

static void byteStreamReturnAscii_FullBuffer_Setup(void **state){
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

static void byteStreamReturnAscii_FailOnDoubleNull_Setup(void **state){
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

static void byteStreamReturnAscii_FailWithNoBytes_Setup(void **state){
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

static void byteStreamReturnUtf16_Word_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "\0u\0t\0f\0\0\0w\0o\0r\0d\0\0\0t\0e\0s\0t\0\0\0";
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

static void byteStreamReturnUtf16_FullBuffer_Setup(void **state){
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

static void byteStreamReturnInt_4bytes_Setup(void **state){
    (void) state; //unused

    ByteStream *stream = byteStreamCreate(NULL, 10);
    byteStreamWrite(stream, (unsigned char *)"\0\0\x01\xa4",4);
    byteStreamSeek(stream, 0, SEEK_SET);
    int d = byteStreamReturnInt(stream);
    assert_int_equal(d,420);
    byteStreamDestroy(stream);
}

static void byteStreamReturnInt_smallStream_Setup(void **state){
    (void) state; //unused

    ByteStream *stream = byteStreamCreate(NULL, 2);
    byteStreamWrite(stream, (unsigned char *)"\x01\xa4",2);
    byteStreamSeek(stream, 0, SEEK_SET);
    int d = byteStreamReturnInt(stream);
    assert_int_equal(d,420);
    byteStreamDestroy(stream);
}

static void byteStreamReturnInt_Fail_Setup(void **state){
    (void) state; //unused
    int d = byteStreamReturnInt(NULL);
    assert_int_equal(d,0);
}

static void byteStreamReturnSyncInt_4bytes_Setup(void **state){
    (void) state; //unused 9992
    ByteStream *stream = byteStreamCreate(NULL, 10);
    byteStreamWrite(stream, (unsigned char *)"\0\0\x27\x08", 4);
    byteStreamSeek(stream, 0, SEEK_SET);
    int d = byteStreamReturnSyncInt(stream);
    assert_int_equal(d,5000);
    byteStreamDestroy(stream);
}

static void byteStreamReturnSize_t_4byte(void **state){
    (void) state;

    if(sizeof(size_t) >= 4){

        ByteStream *stream = byteStreamCreate(NULL, 4);

        byteStreamWrite(stream, (unsigned char *)"\x04\x1B\x7B\x33", 4);
        byteStreamRewind(stream);

        size_t d = byteStreamReturnSize_t(stream);

        assert_int_equal(d, 68909875);
        byteStreamDestroy(stream);

    }

}

static void byteStreamReturnSize_t_6byte(void **state){
    (void) state;

    if(sizeof(size_t) >= 6){

        ByteStream *stream = byteStreamCreate(NULL, 6);

        byteStreamWrite(stream, (unsigned char *)"\x3E\xAC\x54\xAC\xD2\x01", 6);
        byteStreamRewind(stream);

        size_t d = byteStreamReturnSize_t(stream);

        assert_int_equal(d, 68909875909121);
        byteStreamDestroy(stream);

    }

}

static void byteStreamReturnSize_t_3byte(void **state){
    (void) state;

    if(sizeof(size_t) >= 3){

        ByteStream *stream = byteStreamCreate(NULL, 3);

        byteStreamWrite(stream, (unsigned char *)"\xFF\x\xFF\xFF", 3);
        byteStreamRewind(stream);

        size_t d = byteStreamReturnSize_t(stream);

        assert_int_equal(d,16777215);
        byteStreamDestroy(stream);

    }

}

static void byteStreamReturnSyncInt_smallStream_Setup(void **state){
    (void) state; //unused
    ByteStream *stream = byteStreamCreate(NULL, 2);
    byteStreamWrite(stream, (unsigned char *)"\x27\x08", 2);
    byteStreamSeek(stream, 0, SEEK_SET);
    int d = byteStreamReturnSyncInt(stream);
    assert_int_equal(d,5000);
    byteStreamDestroy(stream);
}

static void byteStreamReturnSyncInt_Fail_Setup(void **state){
    (void) state; //unused
    int d = byteStreamReturnSyncInt(NULL);
    assert_int_equal(d,0);
}

static void byteStreamRewind_Normal_Setup(void **state){
    (void) state; //unused

    unsigned char b[] = "u\0t\0f\0 \0w\0o\0r\0d\0 \0t\0e\0s\0t\0\0";
    size_t s = 30;
    ByteStream *stream = byteStreamCreate(b, s);
    byteStreamSeek(stream, 15, SEEK_SET);
    bool v = byteStreamRewind(stream);
    assert_true(v);
    assert_int_equal(stream->cursor,0);
    byteStreamDestroy(stream);
}

static void byteStreamRewind_Normal2_Setup(void **state){
    (void) state; //unused

    ByteStream *stream = byteStreamCreate(NULL, 10);
    byteStreamSeek(stream, 10, SEEK_SET);
    bool v = byteStreamRewind(stream);
    assert_true(v);
    assert_int_equal(stream->cursor,0);
    byteStreamDestroy(stream);
}

static void byteStreamRewind_Fail_Setup(void **state){
    (void) state; //unused

    bool v = byteStreamRewind(NULL);
    assert_false(v);
}

static void byteStreamTell_Normal_Setup(void **state){
    (void) state; //unused
    ByteStream *stream = byteStreamCreate(NULL, 10);
    
    assert_int_equal(byteStreamTell(stream),0);

    byteStreamSeek(stream, 5, SEEK_SET);

    assert_int_equal(byteStreamTell(stream),5);

    byteStreamDestroy(stream);
}

static void byteStreamTell_Fail_Setup(void **state){
    (void) state; //unused

    assert_int_equal(byteStreamTell(NULL),EOF);
    
}

static void byteStreamReadBit_readFirstBit_Setup(void **state){
    (void) state; //unused
    ByteStream *stream = byteStreamCreate(NULL,2);
    
    unsigned char bits = 0b10000010;
    byteStreamWrite(stream, &bits, 1);
    byteStreamRewind(stream);

    assert_int_equal(0, byteStreamReadBit(stream, 0));
    assert_int_equal(1, byteStreamReadBit(stream, 1));
    assert_int_equal(0, byteStreamReadBit(stream, 2));
    assert_int_equal(0, byteStreamReadBit(stream, 3));
    assert_int_equal(0, byteStreamReadBit(stream, 4));
    assert_int_equal(0, byteStreamReadBit(stream, 5));
    assert_int_equal(0, byteStreamReadBit(stream, 6));
    assert_int_equal(1, byteStreamReadBit(stream, 7));

    byteStreamDestroy(stream);
}

static void byteStreamReadBit_ReadIntoSecondByte_Setup(void **state){
    (void) state; //unused
    ByteStream *stream = byteStreamCreate(NULL,2);
    
    unsigned char bits = 0b10000010;
    byteStreamWrite(stream, &bits, 1);

    bits = 0b00011000;
    byteStreamWrite(stream, &bits, 1);
    byteStreamRewind(stream);

    assert_int_equal(byteStreamReadBit(stream, 15), 0);

    byteStreamDestroy(stream);
}

static void byteStreamReadBit_readToFar_Setup(void **state){
    (void) state; //unused
    ByteStream *stream = byteStreamCreate(NULL,2);
    
    unsigned char bits = 0b10000010;
    byteStreamWrite(stream, &bits, 1);

    bits = 0b00011000;
    byteStreamWrite(stream, &bits, 1);
    byteStreamRewind(stream);

    assert_int_equal(byteStreamReadBit(stream, 100), -1);

    byteStreamDestroy(stream);
}

static void byteStreamWriteBit_setAllBits_Setup(void **state){
    (void) state; //unused
    ByteStream *stream = byteStreamCreate(NULL,2);
    
    unsigned char bits = 0b10000010;
    byteStreamWrite(stream, &bits, 1);

    bits = 0b00011000;
    byteStreamWrite(stream, &bits, 1);
    byteStreamRewind(stream);

    for(int i = 0; i < 16; i++){
        assert_true(byteStreamWriteBit(stream, 1, i));
    }

    for(int i = 0; i < 16; i++){
        assert_int_equal(byteStreamReadBit(stream, i), 1);
    }

    byteStreamDestroy(stream);
}

static void byteStreamWriteBit_ReadOutOfRange_Setup(void **state){
    (void) state; //unused
    ByteStream *stream = byteStreamCreate(NULL,2);
    
    unsigned char bits = 0b10000010;
    byteStreamWrite(stream, &bits, 1);

    bits = 0b00011000;
    byteStreamWrite(stream, &bits, 1);
    byteStreamRewind(stream);

    assert_false(byteStreamWriteBit(stream, 1, 99));

    byteStreamDestroy(stream);
}

static void byteStreamDeleteCh_deleteWholeStream_Setup(void **state){
    (void) state; //unused
    
    ByteStream *stream = byteStreamCreate(NULL,10);
    
    for(int i = 0; i < 10; i++){
        assert_true(byteStreamDeleteCh(stream));
    }

    byteStreamDestroy(stream);
}

int main(){
    
    const struct CMUnitTest tests[] = {
        //byteStreamFromFile tests
        cmocka_unit_test(byteStreamFromFile_Normal_Setup),
        cmocka_unit_test(byteStreamFromFile_Fail_Setup),
        //byteStreamCreate tests
        cmocka_unit_test(byteStreamCreate_Normal_Setup),
        cmocka_unit_test(byteStreamCreate_Normal2_Setup),
        cmocka_unit_test(byteStreamCreate_SmallSize_Setup),
        cmocka_unit_test(byteStreamCreate_NoSize_Setup),
        //byteStreamFree tests
        cmocka_unit_test(byteStreamFree_Normal_Setup),
        cmocka_unit_test(byteStreamFree_NoBufferFree_Setup),
        //byteStreamResize tests
        cmocka_unit_test(byteStreamResize_Increase_Setup),
        cmocka_unit_test(byteStreamResize_Decrease_Setup),
        cmocka_unit_test(byteStreamResize_Fail_Setup),
        //byteStreamRead tests
        cmocka_unit_test(byteStreamRead_ReadNothing_Setup),
        cmocka_unit_test(byteStreamRead_ReadTooMuch_Setup),
        cmocka_unit_test(byteStreamRead_ReadWithMaxCursor_Setup),
        cmocka_unit_test(byteStreamRead_HalfBlock_Setup),
        cmocka_unit_test(byteStreamRead_Normal_Setup),
        //byteStreamSeek tests
        cmocka_unit_test(byteStreamSeek_Set_Setup),
        cmocka_unit_test(byteStreamSeek_SetBig_Setup),
        cmocka_unit_test(byteStreamSeek_Cur_Setup),
        cmocka_unit_test(byteStreamSeek_CurBig_Setup),
        cmocka_unit_test(byteStreamSeek_EndBig_Setup),
        cmocka_unit_test(byteStreamSeek_End_Setup),
        //byteStreamReadUntil test
        cmocka_unit_test(byteStreamReadUntil_SpecificCharacter_Setup),
        cmocka_unit_test(byteStreamReadUntil_CharacterNotFound_Setup),
        //byteStreamSearchAndReplace test
        cmocka_unit_test(byteStreamSearchAndReplace_SearchAndReplace_Setup),
        cmocka_unit_test(byteStreamSearchAndReplace_NotFound_Setup),
        cmocka_unit_test(byteStreamSearchAndReplace_ShortPattern_Setup),
        cmocka_unit_test(byteStreamSearchAndReplace_ShortReplace_Setup),
        //byteStreamCursor test
        cmocka_unit_test(byteStreamCursor_Normal_Setup),
        //byteStreamGetCh test
        cmocka_unit_test(byteStreamGetCh_Normal_Setup),
        cmocka_unit_test(byteStreamGetCh_EOF_Setup),
        //byteStreamWrite
        cmocka_unit_test(byteStreamWrite_Normal_Setup),
        cmocka_unit_test(byteStreamWrite_AtMid_Setup),
        cmocka_unit_test(byteStreamWrite_AtEnd_Setup),
        cmocka_unit_test(byteStreamWrite_Err_Setup),
        //byteStreamWriteAtPosition
        cmocka_unit_test(byteStreamWriteAtPosition_Err_Setup),
        cmocka_unit_test(byteStreamWriteAtPosition_Start_Setup),
        cmocka_unit_test(byteStreamWriteAtPosition_Mid_Setup),
        
        //byteStreamReadBit
        cmocka_unit_test(byteStreamReadBit_readFirstBit_Setup),
        cmocka_unit_test(byteStreamReadBit_ReadIntoSecondByte_Setup),
        cmocka_unit_test(byteStreamReadBit_readToFar_Setup),

        //byteStreamWriteBit
        cmocka_unit_test(byteStreamWriteBit_setAllBits_Setup),
        cmocka_unit_test(byteStreamWriteBit_ReadOutOfRange_Setup),

        //byteStreamReturnAscii
        //same function is called for:
        // - byteStreamReturnLatin1
        // - byteStreamReturnUtf8
        cmocka_unit_test(byteStreamReturnAscii_Word_Setup),
        cmocka_unit_test(byteStreamReturnAscii_FullBuffer_Setup),
        cmocka_unit_test(byteStreamReturnAscii_FailOnDoubleNull_Setup),
        cmocka_unit_test(byteStreamReturnAscii_FailWithNoBytes_Setup),
        //byteStreamReturnUtf16
        cmocka_unit_test(byteStreamReturnUtf16_Word_Setup),
        cmocka_unit_test(byteStreamReturnUtf16_FullBuffer_Setup),
        
        //byteStreamReturnInt
        cmocka_unit_test(byteStreamReturnInt_4bytes_Setup),
        cmocka_unit_test(byteStreamReturnInt_smallStream_Setup),
        cmocka_unit_test(byteStreamReturnInt_Fail_Setup),
        //byteStreamReturnSyncInt
        cmocka_unit_test(byteStreamReturnSyncInt_4bytes_Setup),
        cmocka_unit_test(byteStreamReturnSyncInt_smallStream_Setup),
        cmocka_unit_test(byteStreamReturnSyncInt_Fail_Setup),

        //byteStreamReturnSize_t
        cmocka_unit_test(byteStreamReturnSize_t_4byte),
        cmocka_unit_test(byteStreamReturnSize_t_6byte),
        cmocka_unit_test(byteStreamReturnSize_t_3byte),

        /**
         * u32 function not tested as it has the same logic as byteStreamReturnInt
         * 
         */


        //byteStreamRewind
        cmocka_unit_test(byteStreamRewind_Normal_Setup),
        cmocka_unit_test(byteStreamRewind_Normal2_Setup),
        cmocka_unit_test(byteStreamRewind_Fail_Setup),
        //byteStreamTell
        cmocka_unit_test(byteStreamTell_Normal_Setup),
        cmocka_unit_test(byteStreamTell_Fail_Setup),

        // byteStreamDeleteCh
        cmocka_unit_test(byteStreamDeleteCh_deleteWholeStream_Setup)

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}