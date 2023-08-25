/**
 * @file byteStream.c
 * @author Ewan Jones
 * @brief functions for creating and performing operations on a ByteStream structure
 * @version 1.0.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "byteDefines.h"
#include "byteStream.h"
#include "byteTypes.h"
#include "byteUnicode.h"
#include "byteInt.h"

/**
 * @brief Creates a ByteStream stucture from a provided file or path
 * @param fileName 
 * @return ByteStream* 
 */
ByteStream *byteStreamFromFile(const char *fileName){

    FILE *fp = NULL;

    if(!(fp = fopen(fileName, "rb"))){
        return NULL;
    }

    //get filesize
    fseek(fp, 0L, SEEK_END);
    size_t sz = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    //no bytes were read
    if(!sz){
        fclose(fp);
        return NULL;
    }

    unsigned char *tmp = malloc(sz);

    if(!(fread(tmp, sz, sizeof(unsigned char), fp))){
        free(tmp);
        fclose(fp);
        return NULL;
    }

    ByteStream *stream = byteStreamCreate(tmp, sz);
    free(tmp);
    fclose(fp);
    return stream;
}

/**
 * @brief Creates a ByteStream structure
 * @details Creates a stream with the provided buffer of n size. If
 * NULL is given for the buffer parameter this function will create a ByteStream
 * initilized with 0s.
 * @param buffer 
 * @param n
 * @return ByteStream* 
 */
ByteStream *byteStreamCreate(unsigned char *buffer, size_t n){
    
    if(!n){
        return NULL;
    }

    ByteStream *stream = malloc(sizeof(ByteStream));
    unsigned char *tmp = malloc(n + BYTE_PADDING);
    memset(tmp, 0, n + BYTE_PADDING);
    
    if(buffer){
        memcpy(tmp, buffer, n);
    }

    stream->buffer = tmp;
    stream->bufferSize = n;
    stream->cursor = 0;

    return stream;
}

/**
 * @brief Resizes a stream by n
 * @details This function will insert 0s when a greater size is provided however, it
 * will erase data already in the stream if a smaller size is provided.
 * @param stream 
 * @param newBufferSize 
 */
void byteStreamResize(ByteStream *stream, size_t n){

    if(stream == NULL || n == 0){
        return;
    }

    unsigned char *tmp = malloc(n + BYTE_PADDING);
    memset(tmp, 0, n + BYTE_PADDING);

    if(n > stream->bufferSize){
        
        //create a new buffer
        memcpy(tmp, stream->buffer, stream->bufferSize);
        memset(tmp + stream->bufferSize, 0, n - stream->bufferSize);
    
    }else if(n < stream->bufferSize){
        
        //create a new buffer
        memcpy(tmp, stream->buffer, n);

        if(stream->cursor > n){
            stream->cursor = n;
        }

    }else{
        free(tmp);
        return;
    }

    free(stream->buffer);
    stream->buffer = tmp;
    stream->bufferSize = n;
}

/**
 * @brief Frees internal variables in a ByteStream structure
 * @details This frees memory allocated to the buffer and will set its size and position to 0
 * @param toDelete 
 */
void byteStreamFree(ByteStream *toDelete){

    if(toDelete == NULL){
        return;
    }

    if(toDelete->buffer){
        free(toDelete->buffer);
        toDelete->buffer = NULL;
    }

    toDelete->cursor = 0;
    toDelete->bufferSize = 0;

}

/**
 * @brief Frees a ByteStream and destroys its structure
 * @param toDelete 
 */
void byteStreamDestroy(ByteStream *toDelete){
    
    if(toDelete != NULL){
        byteStreamFree(toDelete);
        free(toDelete);
        toDelete = NULL;
    }
}

/**
 * @brief Reads n bytes in the stream and updates dest
 * @details This function returns true if a read of n bytes is successful
 * other wise it will return false.
 * @param stream 
 * @param dest 
 * @param size 
 * @return true 
 * @return false 
 */
bool byteStreamRead(ByteStream *stream, unsigned char *dest, size_t n){

    if(stream == NULL || n == 0){
        return false;
    }

    size_t wSize = n;

    //prevent overflow
    if(stream->cursor + n > stream->bufferSize){
        wSize = stream->bufferSize - stream->cursor;
    }

    if(wSize == 0){
        return false;
    }
    
    //read
    memcpy(dest, byteStreamCursor(stream), wSize);
    stream->cursor = stream->cursor + wSize;
    return true;
}

/**
 * @brief Seeks dest bytes from a seek option within a stream
 * @details this function will seek from the end of the stream(SEEK_END), start
 * (SEEK_SET), and the current person (SEEK_CUR). If this function is successful
 * it will return true otherwise false.
 * @param stream 
 * @param dest 
 * @param seekOption 
 * @return true 
 * @return false 
 */
bool byteStreamSeek(ByteStream *stream, size_t dest, const int seekOption){

    bool ret = false;

    if(stream == NULL){
        return ret;
    }
    
    switch(seekOption){
        
        //reset to the beginning
        case SEEK_SET:
            if(!(dest > stream->bufferSize)){
                stream->cursor = dest;
                ret = true;
            }
            break;

        //use dest as new cursor    
        case SEEK_CUR:
            if(!(stream->cursor + dest > stream->bufferSize)){
                stream->cursor = stream->cursor + dest;
                ret = true;
            }

            break;
        
        //set cursor to end of buffer
        case SEEK_END:
            if(!(dest > stream->bufferSize)){
                stream->cursor = stream->bufferSize - dest;
                ret = true;
            }

            break;

        default:
            break;
    }

    return ret;
}

/**
 * @brief Reads until a delimiter is found and returns it to the caller.
 * @details If the delimiter is not found null will be returned.
 * @param stream 
 * @param delimiter 
 * @return unsigned char* 
 */
unsigned char *byteStreamReadUntil(ByteStream *stream, unsigned char delimiter){

    if(stream == NULL){
        return NULL;
    }
    
    size_t j = 0;

    for(size_t i = stream->cursor; i < stream->bufferSize; i++){
        
        j++;

        //create return
        if(stream->buffer[i] == delimiter){
            unsigned char *ret = malloc(j + BYTE_PADDING);
            
            memset(ret, 0, j + BYTE_PADDING);
            byteStreamRead(stream, ret, j);
            return ret;
        }
    }
    
    return NULL;
}

/**
 * @brief Uses a pattern to replace all instances with a replace sequence.
 * @details If the pattern is shorter then the replace sequence the entire replace sequence
 * will not be copied. The same same is true for the reverse of the two sequenses.
 * @param stream 
 * @param pattern 
 * @param patternSize 
 * @param replace 
 * @param replaceSize 
 */
void byteStreamSearchAndReplace(ByteStream *stream, unsigned char *pattern, size_t patternSize, unsigned char *replace, size_t replaceSize){

    if(stream == NULL || pattern == NULL || patternSize == 0){
        return;
    }

    //preprocess
    size_t table[BYTE_HORSPOOL_TABLE_SIZE];
    
    //makes the shift table
    for(int i = 0; i < BYTE_HORSPOOL_TABLE_SIZE; i++){
        table[i] = patternSize;
    }

    //calculate the shift values for horspool
    for(int i = 0; i < patternSize - 1; i++){
        table[(size_t)pattern[i]] = patternSize - i - 1;
    }

    //horspool algorithm
    size_t i = patternSize - 1;
    while(i < stream->bufferSize){
        
        size_t k = 0;

        while((k < patternSize) && (pattern[patternSize - 1 - k] == stream->buffer[i - k])){
            k++;
        }

        //replace match
        if(k == patternSize){
            for (size_t j = 0; j < patternSize; j++) {
                if(j < replaceSize){
                    stream->buffer[(i - patternSize + 1) + j] = replace[j];
                }else{
                    stream->buffer[(i - patternSize + 1) + j] = 0;
                }
            }
        }
          
        i = i + table[(size_t)stream->buffer[i]];
    }
}

/**
 * @brief Returns a pointer at the current position in the stream.
 * @details If the cursor is at the end of the stream this function will return null.
 * @param stream 
 * @return unsigned char* 
 */
unsigned char *byteStreamCursor(ByteStream *stream){
    
    if(stream == NULL){
        return NULL;
    }
   
    if(stream->cursor >= stream->bufferSize){
        return NULL;
        
    }

    return stream->buffer + stream->cursor;
}

/**
 * @brief Returns the current byte at the streams current position.
 * @details If the end of the stream is reached this fucntion will return -1 or EOF.
 * @param stream 
 * @return int 
 */
int byteStreamGetCh(ByteStream *stream){

    if(stream == NULL){
        return EOF;
    }

    unsigned char *c = byteStreamCursor(stream);
    
    //control output
    return c == NULL ? EOF: c[0];
}

/**
 * @brief Writes n bytes of src to the current position in the stream.
 * @details This function will return false if it was unsuccessful in writing
 * to the stream otherwise it will return true.
 * @param stream 
 * @param src 
 * @param n 
 * @return true 
 * @return false 
 */
bool byteStreamWrite(ByteStream *stream, unsigned char *src, size_t n){

    if(stream == NULL || src == NULL || n > stream->bufferSize || n  == 0){
        return false;
    }

    size_t wSize = n;

    //prevent overflow
    if(n + stream->cursor > stream->bufferSize){
        wSize = stream->bufferSize - stream->cursor;
    }

    //write
    memcpy(stream->buffer + stream->cursor, src, wSize);
    stream->cursor += wSize;
    return true;
}

/**
 * @brief Writes n bytes of src to a provided position in the stream.
 * @details This function will return false if it was unsuccessful in writing
 * to the stream otherwise it will return true. This function will not update the
 * current position of the stream iteslf and only relies on the pos provided.
 * @param stream 
 * @param src 
 * @param n 
 * @param pos 
 * @return true 
 * @return false 
 */
bool byteStreamWriteAtPosition(ByteStream *stream, unsigned char *src, size_t n, size_t pos){

    if(stream == NULL || pos > stream->bufferSize || src == NULL){
        return false;
    }

    size_t wSize = n;

    //prevent overflow
    if(n + pos > stream->bufferSize){
        wSize = stream->bufferSize - pos;
    }

    memcpy((stream->buffer) + pos, src, wSize);
    return true;

}

/**
 * @brief Reads and returns the first ascii string in the current stream and provides its length.
 * @details This function is not strict and only looks for the first occurance of a 0 byte meaning
 * it can be used with other sequences. If this function fails it will return null otherise some
 * form of null terminated data will be returned.
 * @details this function 
 * @param stream 
 * @param outLen 
 * @return unsigned char* 
 */
unsigned char *byteStreamReturnAscii(ByteStream *stream, size_t *outLen){

    if(stream == NULL){
        *outLen = 0;
        return NULL;
    }

    size_t len = byteStrlen(BYTE_ASCII, byteStreamCursor(stream));

    if(len){
        len++;
        unsigned char *ret = calloc(sizeof(unsigned char), len + BYTE_PADDING);
        byteStreamRead(stream, ret, len);
        *outLen = len;
        return ret;   
    }

    //read no bytes
    *outLen = 0;
    return NULL;
}

/**
 * @brief Reads and returns the first latin1 string in the current stream and provides its length.
 * @details This function is not strict and only looks for the first occurance of a 0 byte meaning
 * it can be used with other sequences. If this function fails it will return null otherise some
 * form of null terminated data will be returned.
 * @param stream 
 * @param outLen 
 * @return unsigned char* 
 */
unsigned char *byteStreamReturnLatin1(ByteStream *stream, size_t *outLen){
    //just extended ascii and its terminated the same way
    return byteStreamReturnAscii(stream, outLen);
}

/**
 * @brief Reads and returns the first utf8 string in the current stream and provides its length.
 * @details This function is not strict and only looks for the first occurance of a 0 byte meaning
 * it can be used with other sequences. If this function fails it will return null otherise some
 * form of null terminated data will be returned.
 * @param stream 
 * @param outLen 
 * @return unsigned char* 
 */
unsigned char *byteStreamReturnUtf8(ByteStream *stream, size_t *outLen){
    //terminated the same way
    return byteStreamReturnAscii(stream, outLen);
}

/**
 * @brief Reads and returns the first utf16be or utf16le string in the current stream and provides its length.
 * @details This function is not strict and only looks for the first occurance of a 00 bytes meaning
 * it can be used with other sequences. If this function fails it will return null otherise some
 * form of null terminated data will be returned.
 * @param stream 
 * @param outLen 
 * @return unsigned char* 
 */
unsigned char *byteStreamReturnUtf16(ByteStream *stream, size_t *outLen){

    if(stream == NULL){
        *outLen = 0;
        return NULL;
    }

    //would work the same if LE was given instead
    size_t len = byteStrlen(BYTE_UTF16BE, byteStreamCursor(stream));

    if(len){
        unsigned char *ret = calloc(sizeof(unsigned char), len + BYTE_PADDING);
        byteStreamRead(stream, ret, len + 2);   
        *outLen = len;
        return ret;
    }

    //read no bytes
    *outLen = 0;
    return NULL;
}

/**
 * @brief Reads an integer from a stream 
 * 
 * @param stream 
 * @return int 
 */
int byteStreamReturnInt(ByteStream *stream){

    if(stream == NULL){
        return 0;
    }

    //an int is 4 bytes but....
    int is = 4;
    unsigned char buff[4] = {0,0,0,0};

    for(int i = 0; i < 4; i++){
        if(stream->cursor + i == stream->bufferSize){
            is = i;
            break;
        }
    }
    
    if(!(byteStreamRead(stream, buff, is))){
        return 0;
    }


    return btoi(buff,is);
}

unsigned int byteStreamReturnSyncInt(ByteStream *stream){
    return byteSyncintDecode(byteStreamReturnInt(stream));
}

/**
 * @brief Prints from the current position of a stream with %x, %c, or %d.
 * @param formatSpecifier 
 * @param stream 
 */
void byteStreamPrintf(const char *formatSpecifier, ByteStream *stream){

    if(stream == NULL){
        printf("\n");
        return;
    }

    if(stream->cursor == stream->bufferSize){
        printf("[]\n");
        return;
    }

    size_t size = stream->bufferSize - stream->cursor;
    
    if(size == 0){
        printf("[]\n");
        return;
    }

    printf("[");
    for(size_t i = stream->cursor; i < stream->bufferSize; i++){
        if(strncmp("%x", formatSpecifier, 3) == 0){
            printf("[%x]",stream->buffer[i]);
        }else if(strncmp("%c", formatSpecifier, 3) == 0){
            printf("[%c]",stream->buffer[i]);   
        }else if(strncmp("%d", formatSpecifier, 3) == 0){
            printf("[%d]",stream->buffer[i]);
        }
    }
    printf("]\n");
}

/**
 * @brief Rewinds a stream to its starting position.
 * @param stream 
 * @return true 
 * @return false 
 */
bool byteStreamRewind(ByteStream *stream){
    if(stream == NULL){
        return false;
    }

    stream->cursor = 0;

    return true;
}

/**
 * @brief Provides the number of bytes the caller is from the beginning of the stream.
 * @details This function will return EOF or -1 if it fails to read the provided stream otherwise, it will return a positive value. 
 * @param stream 
 * @return long int 
 */
long int byteStreamTell(ByteStream *stream){
    if(stream == NULL){
        return EOF;
    }

    return stream->cursor;
}
