#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <byteStream.h>
#include <byteUnicode.h>

#define MAX_SIZE 15000
#define W 100

int main(int argc, char *argv[]){
    if(argc != 2){
        perror("usage: ./main <file>");
        exit(EXIT_FAILURE);
    }

    unsigned char *fileContent = NULL;
    size_t fileSize = 0;
    FILE *fp = NULL;
    ByteStream *stream = NULL;
    
    
    if((fp = fopen(argv[1], "rb")) == NULL){
        perror("failed to open file");
        exit(EXIT_FAILURE);
    }

    fseek(fp, 0L, SEEK_END);
    fileSize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    fileContent = calloc(sizeof(unsigned char), fileSize);
    if((fread(fileContent, sizeof(unsigned char), fileSize, fp)) == 0){
        perror("failed to read from file");
        fclose(fp);
        free(fileContent);
        exit(EXIT_FAILURE);
    }

    stream = byteStreamCreate(fileContent, fileSize);
    free(fileContent);
    
    //byteStreamSearchAndReplace(stream, (unsigned char *)" ", 1, 0x00);

    while(byteStreamGetCh(stream) != EOF){
        size_t inlen = 0;
        size_t outlen = 0;
        unsigned char *str = byteStreamReturnAscii(stream, &inlen);//byteStreamReturnUtf16(stream, &len);
        unsigned char *newStr = NULL;

        if(str != NULL && inlen > 0){
            
            //convert
            outlen = 4 * inlen;
            bool v = byteConvertTextFormat(str, BYTE_ASCII, &inlen, &newStr, BYTE_UTF16LE, &outlen);
            free(str);
            /*
            if(newStr != NULL && v == true){
                fwrite(newStr, 1, outlen - BYTE_PADDING, fp );
                fclose(fp);
                free(newStr);
            }else if(newStr != NULL){
                free(newStr);
            }
            */
        }

        
    }
    
    byteStreamDestroy(stream);
    return 0;
}
