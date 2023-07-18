#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "byteDefines.h"
#include "byteStream.h"
#include "byteTypes.h"

ByteStream *byteStreamCreate(unsigned char *buffer, size_t bufferSize){

    if(buffer == NULL){
        return NULL;
    }
    
    if(bufferSize == 0){
        return NULL;
    }


    ByteStream *stream = malloc(sizeof(ByteStream));

    stream->buffer = calloc(sizeof(unsigned char), bufferSize);
    stream->bufferSize = bufferSize;
    stream->cursor = 0;

    memcpy(stream->buffer, buffer, bufferSize);
    return stream;
}

void byteStreamFree(ByteStream *toDelete){

    if(toDelete == NULL){
        return;
    }

    if(toDelete->buffer){
        free(toDelete->buffer);
        toDelete->cursor = 0;
        toDelete->bufferSize = 0;
    }
}

void byteStreamDestroy(ByteStream *toDelete){
    
    if(toDelete != NULL){
        byteStreamFree(toDelete);
        free(toDelete);
    }
}

void byteStreamRead(ByteStream *stream, unsigned char *dest, size_t size){

    if(stream == NULL){
        dest = NULL;
        return;
    }

    if(size == 0){
        dest = NULL;
        return;
    }
    if(size > stream->bufferSize){
        dest = NULL;
        return;
    }

    memcpy(dest, id3ReaderCursor(stream), size);
    stream->cursor = stream->cursor + size;
}

void byteStreamSeek(ByteStream *stream, size_t dest, const int seekOption){

    if(stream == NULL){
        return;
    }
    
    switch(seekOption){
        
        //reset to the beginning
        case SEEK_SET:
            stream->cursor = dest;
            break;

        //use dest as new cursor    
        case SEEK_CUR:
            stream->cursor = stream->cursor + dest;
            break;
        
        //set cursor to end of buffer
        case SEEK_END:
            stream->cursor = stream->bufferSize - dest;
            break;

        default:
            break;
    }

}

void byteStreamReadUntil(ByteStream *stream, unsigned char *dest, unsigned char delimiter){

    if(stream == NULL){
        return;
    }

    size_t got = 0;
    unsigned char *ret = NULL;

    for(size_t i = stream->cursor; i < stream->bufferSize; i++){
        if(stream->buffer[i] == delimiter){
            byteStreamRead(stream, ret, i);
        }
    }

    

    return ret;
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

unsigned char byteStreamGetCh(ByteStream *stream){

    if(stream == NULL){
        return EOF;
    }

    return byteStreamCursor(stream) == NULL ? EOF: byteStreamCursor(stream)[0];

}