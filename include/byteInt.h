/**
 * @file byteInt.h
 * @author Ewan Jones
 * @brief header for integer functions
 * @version 0.1
 * @date 2023-08-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef BYTE_INT
#define BYTE_INT

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>
#include <stdbool.h>

int btoi(unsigned char *bytes, int byteNum);
char *itob(int i);

uint8_t setBit(uint8_t n, unsigned int k, bool v);
int readBit(uint8_t n, unsigned int k);

uint64_t byteSyncintDecode(uint64_t value);
uint64_t byteSyncintEncode(uint64_t value);

#ifdef __cplusplus
} //extern c end
#endif

#endif