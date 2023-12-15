/**
 * @file byteTypes.h
 * @author Ewan Jones
 * @brief contains types used by the library
 * @version 1.0.0
 * @date 2023-08-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef BYTE_TYPES
#define BYTE_TYPES

#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>
#include <stdint.h>
#include "byteDefines.h"

/**
 * @brief A structure made to hold a block of memory and keep track of its size and the creators index  
 * 
 */
typedef struct _ByteStream{
    //!A block of memory
    uint8_t *buffer;
    
    //!The size of the buffer
    size_t bufferSize; 
    
    //! The current position in the buffer
    size_t cursor; 
    
}ByteStream;

#ifdef __cplusplus
} //extern c end
#endif

#endif