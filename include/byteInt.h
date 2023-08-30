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
#ifndef BYTE_ENDIAN
#define BYTE_ENDIAN

#ifdef __cplusplus
extern "C"{
#endif

int btoi(unsigned char *bytes, int byteNum);
char *itob(int i);

unsigned int byteSyncintDecode(unsigned int value);
unsigned int byteSyncintEncode(unsigned int value);

#ifdef __cplusplus
} //extern c end
#endif

#endif