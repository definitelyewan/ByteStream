/**
 * @file byteInt.c
 * @author Ewan Jones
 * @brief Contains functions for usign different kinds of integers and converting them to bytes
 * @version 1.0
 * @date 2023-08-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include "byteEndian.h"
#include "byteInt.h"


/**
 * @brief Converts byteNum bytes from bytes into a signed integer.
 * @param bytes 
 * @param byteNum 
 * @return int 
 */
int btoi(unsigned char *bytes, int byteNum){
    
    int byteAsInt = 0x00;
    for(int i = 0; i < byteNum; i++){
        byteAsInt = byteAsInt << 8;
        byteAsInt = byteAsInt | (unsigned char) bytes[i];
    }

    return byteAsInt;	  
}

/**
 * @brief Converts an integer to its representation as a signed char*.
 * @param i 
 * @return char* 
 */
char *itob(int i){

    int sz = 4;
    char *ret = malloc(sizeof(char) * sz);
    char *aux = (char *) &i;
    
    //big endian
    if(!byteEndianess()){
        for(int j = 0; j < sz; j++){
            ret[j] = aux[j];
        }

    //little endian
    }else{
        //reserve
        for(int j = sz - 1; j >= 0; j--){
            ret[sz - 1 - j] = aux[j];
        }

    }

    return ret;
}

/**
 * @brief Converts a 4 byte, 7 bit sync integer to its representation as a unsigned integer. 
 * @details A SafeSync integers byte format looks as follows 0xxxxxxx where x represents a bit.
 * @param value 
 * @return unsigned int 
 */
unsigned int byteSyncintDecode(unsigned int value){

    unsigned int result = 0x00;
    unsigned int a = value & 0xFF;
    unsigned int b = (value >> 8) & 0xFF;
    unsigned int c = (value >> 16) & 0xFF;
    unsigned int d = (value >> 24) & 0xFF;

    result = result | a;
    result = result | (b << 7);
    result = result | (c << 14);
    result = result | (d << 21);

    return result;
    
}

/**
 * @brief Converts an unsigned value into its representation as a byte, 7 bit sync integer.
 * @details A SafeSync integers byte format looks as follows 0xxxxxxx where x represents a bit.
 * @param value 
 * @return unsigned int 
 */
unsigned int byteSyncintEncode(unsigned int value){
    
    int out = 0x7F;
    int mask = 0x7F;

    while(mask ^ 0x7FFFFFFF){
        out = value & ~mask;
        out <<= 1;
        out |= value & mask;
        mask = ((mask + 1) << 8) - 1;
        value = out;
    }

    return out;
}