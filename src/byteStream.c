#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <byteDefines.h>
#include <byteStream.h>
#include <byteTypes.h>
#include <byteUnicode.h>
#include <assert.h>


ByteStream *byteStreamFromFile(const char *fileName){

    assert(fileName);

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

ByteStream *byteStreamCreate(unsigned char *buffer, size_t bufferSize){
    
    if(!bufferSize){
        return NULL;
    }

    ByteStream *stream = malloc(sizeof(ByteStream));
    unsigned char *tmp = calloc(sizeof(unsigned char), bufferSize + BYTE_PADDING);
    
    if(buffer){
        memcpy(tmp, buffer, bufferSize);
    }

    stream->buffer = tmp;
    stream->bufferSize = bufferSize;
    stream->cursor = 0;

    return stream;
}

void byteStreamResize(ByteStream *stream, size_t newBufferSize){

    if(stream == NULL || newBufferSize == 0){
        return;
    }

    unsigned char *tmp = calloc(sizeof(unsigned char), newBufferSize);

    if(newBufferSize > stream->bufferSize){
        
        //create a new buffer
        memcpy(tmp, stream->buffer, stream->bufferSize);
        memset(tmp + stream->bufferSize, 0, newBufferSize - stream->bufferSize);
    
    }else if(newBufferSize < stream->bufferSize){
        
        //create a new buffer
        memcpy(tmp, stream->buffer, newBufferSize);

        if(stream->cursor > newBufferSize){
            stream->cursor = newBufferSize;
        }

    }else{
        free(tmp);
        return;
    }

    free(stream->buffer);
    stream->buffer = tmp;
    stream->bufferSize = newBufferSize;
}

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

void byteStreamDestroy(ByteStream *toDelete){
    
    if(toDelete != NULL){
        byteStreamFree(toDelete);
        free(toDelete);
        toDelete = NULL;
    }
}

bool byteStreamRead(ByteStream *stream, unsigned char *dest, size_t size){

    if(stream == NULL || size == 0){
        return false;
    }

    size_t wSize = size;

    if(stream->cursor + size > stream->bufferSize){
        wSize = stream->bufferSize - stream->cursor;
    }

    if(wSize == 0){
        return false;
    }
    

    memcpy(dest, byteStreamCursor(stream), wSize);
    stream->cursor = stream->cursor + wSize;
    return true;
}

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
            return ret;
    }

    return ret;
}

unsigned char *byteStreamReadUntil(ByteStream *stream, unsigned char delimiter){

    if(stream == NULL){
        return NULL;
    }

    unsigned char *ret = NULL;

    for(size_t i = stream->cursor; i < stream->bufferSize; i++){
        if(stream->buffer[i] == delimiter){
            ret = calloc(sizeof(unsigned char), i + BYTE_PADDING);
            byteStreamRead(stream, ret, i + 1);
            break;
        }
    }
    
    return ret;
}

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
                    stream->buffer[i - j] = replace[j];
                }else{
                    stream->buffer[i - j] = 0;
                }
            }
        }
          
        i = i + table[(size_t)stream->buffer[i]];
    }
}

unsigned char *byteStreamCursor(ByteStream *stream){
    
    if(stream == NULL){
        return NULL;
    }
   
    if(stream->cursor >= stream->bufferSize){
        return NULL;
        
    }

    return stream->buffer + stream->cursor; 
}

int byteStreamGetCh(ByteStream *stream){

    if(stream == NULL){
        return EOF;
    }

    return byteStreamCursor(stream) == NULL ? EOF: byteStreamCursor(stream)[0];
}

bool byteStreamWrite(ByteStream *stream, unsigned char *src, size_t srcSize){

    if(stream == NULL || src == NULL || srcSize > stream->bufferSize || srcSize  == 0){
        return false;
    }

    size_t wSize = srcSize;

    //prevent overflow
    if(srcSize + stream->cursor > stream->bufferSize){
        wSize = stream->bufferSize - stream->cursor;
    }

    memcpy(stream->buffer + stream->cursor, src, wSize);
    stream->cursor += wSize;
    return true;
}

bool byteStreamWriteAtPosition(ByteStream *stream, unsigned char *src, size_t srcSize, size_t pos){

    if(stream == NULL || pos > stream->bufferSize || src == NULL){
        return false;
    }

    size_t wSize = srcSize;

    //prevent overflow
    if(srcSize + pos > stream->bufferSize){
        wSize = stream->bufferSize - pos;
    }

    memcpy((stream->buffer) + pos, src, wSize);
    return true;

}

unsigned char *byteStreamReturnAscii(ByteStream *stream, size_t *outLen){

    if(stream == NULL){
        *outLen = 0;
        return NULL;
    }

    size_t len = 0;
    unsigned char *ret = NULL;

    len = byteStrlen(BYTE_ASCII, byteStreamCursor(stream));

    if(len){
        len++;
        ret = calloc(sizeof(unsigned char), len + BYTE_PADDING);
        byteStreamRead(stream, ret, len);   
    }

    //read no bytes
    if(ret == NULL){
        len = 0;
    }

    *outLen = len;
    return ret;
}

unsigned char *byteStreamReturnLatin1(ByteStream *stream, size_t *outLen){
    //just extended ascii and its terminated the same way
    return byteStreamReturnAscii(stream, outLen);
}

unsigned char *byteStreamReturnUtf8(ByteStream *stream, size_t *outLen){
    //terminated the same way
    return byteStreamReturnAscii(stream, outLen);
}

unsigned char *byteStreamReturnUtf16(ByteStream *stream, size_t *outLen){

    if(stream == NULL){
        *outLen = 0;
        return NULL;
    }

    size_t len = 0;
    unsigned char *ret = NULL;

    //would work the same if LE was given instead
    len = byteStrlen(BYTE_UTF16BE, byteStreamCursor(stream));

    if(len){
        ret = calloc(sizeof(unsigned char), len + BYTE_PADDING);
        byteStreamRead(stream, ret, len + 1);   
    }

    //read no bytes
    if(ret == NULL){
        len = 0;
    }

    *outLen = len;
    return ret;


}

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
