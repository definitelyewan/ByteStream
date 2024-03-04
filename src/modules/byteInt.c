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
#include <stdint.h>
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
 * @brief Converts a size_t to its representation as a unsigned char *.
 * 
 * @param st 
 * @return unsigned char* 
 */
unsigned char *sttob(size_t st){

    size_t sz = sizeof(size_t);
    unsigned char *ret = malloc(sizeof(unsigned char) * sz);
    unsigned char *aux = (unsigned char *) &st;

    // big endian
    if(!byteEndianess()){
        for(size_t j = 0; j < sz; j++){
            ret[j] = aux[j];
        }
    
    }else{
        for(size_t j = sz - 1; j < sz; j--){
            ret[sz - 1 - j] = aux[j];
        }
    }

    return ret;
}

/**
 * @brief Converts byteNum bytes from bytes into a size_t. 
 * 
 * @param bytes 
 * @param byteNum 
 * @return size_t 
 */
size_t btost(unsigned char *bytes, int byteNum){

    size_t byteAsSize_t = 0U;

    for(int i = 0; i < byteNum; i++){
        byteAsSize_t = byteAsSize_t << 8;
        byteAsSize_t = byteAsSize_t | (unsigned char) bytes[i];
    }

    return byteAsSize_t;
}


/**
 * @brief set the kth bit of n to value v
 * 
 * @param n 
 * @param k 
 * @param v 
 * @return uint8_t 
 */
uint8_t setBit(uint8_t n, unsigned int k, bool v){

    if(k > 7){// 7 is MSB or LSB
        return n;
    }

    //mask out bit
    n = n & ~(1 << k);

    //if v is set bit otherwise, just use the mask
    return (v) ? (n | (1 << k)) : n;
}

/**
 * @brief read the kth bit of n
 * 
 * @param n 
 * @param k 
 * @return unsigned int 
 */
int readBit(uint8_t n, unsigned int k){
  //7 is MSB or LSB. 
  //k > 7 is not possible for a uint8
  return (k > 7) ? -1 : (n >> k) & 1;
}

/**
 * @brief Converts an integer up to 64 bits, 7 bit sync integer to its representation as a unsigned integer. 
 * @details A SafeSync integers byte format looks as follows 0xxxxxxx where x represents a bit.
 * @param value 
 * @return uint64_t 
 */
uint64_t byteSyncintDecode(uint64_t value){
    uint64_t result = 0x00;
    uint64_t a = value & 0xFF;
    uint64_t b = (value >> 8) & 0xFF;
    uint64_t c = (value >> 16) & 0xFF;
    uint64_t d = (value >> 24) & 0xFF;
    uint64_t e = (value >> 32) & 0xFF;
    uint64_t f = (value >> 40) & 0xFF;

    result = result | a;
    result = result | (b << 7);
    result = result | (c << 14);
    result = result | (d << 21);
    result = result | (e << 28);
    result = result | (f << 35);

    return result;
}

/**
 * @brief Converts an unsigned value into its representation as a byte, 7 bit sync integer.
 * @details A SafeSync integers byte format looks as follows 0xxxxxxx where x represents a bit.
 * @param value 
 * @return uint64_t 
 */
uint64_t byteSyncintEncode(uint64_t value){
    
    uint64_t out = 0x7F;
    uint64_t mask = 0x7F;

    while(mask ^ 0x7FFFFFFFFFFFFFFF){
        out = value & ~mask;
        out <<= 1;
        out |= value & mask;
        mask = ((mask + 1) << 8) - 1;
        value = out;
    }

    return out;
}
