/**
 * @file byteEndian.h
 * @author Ewan Jones
 * @brief header for endian functions
 * @version 0.1
 * @date 2023-08-24
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef BYTE_ENDIAN
#define BYTE_ENDIAN

#ifdef __cplusplus
extern "C"{
#endif

#include <stdbool.h>

//system
void byteSetEndianess(bool endianess);
bool byteEndianess();


#ifdef __cplusplus
} //extern c end
#endif

#endif