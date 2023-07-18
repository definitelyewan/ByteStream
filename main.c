#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "byteStream.h"


//w3 ref
//https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwidoOa53ZaAAxVag4kEHVTXDzMQFnoECBwQAQ&url=https%3A%2F%2Fdev.w3.org%2FXML%2Fencoding.c&usg=AOvVaw2LrNRpxyvCE5_LxXUyIeMC&opi=89978449
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
    byteStreamDestroy(stream);

    fclose(fp);
    return 0;
}
