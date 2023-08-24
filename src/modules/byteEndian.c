/**
 * @file byteEndian.c
 * @author Ewan Jones
 * @brief Controls the endianess of all functions that require the endianess of the system.
 * @version 1.0
 * @date 2023-08-23
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "byteEndian.h"

/**
 * @brief The endianess of the system. This variable is used to determin if the conversion functions are to use big or little endian while processing. 
 * 
 */
static bool byteLittleEndian = true;

/**
 * @brief This function is responsible for changing the endianess of
 * encoding conversion functions.
 * 
 * @details Little endian is 1 and big endian is 0.
 * this function has no return value.
 * @param endianess 
 */
void byteSetEndianess(bool endianess){
    byteLittleEndian = endianess;
}

/**
 * @brief returns library endianess
 * 
 * @return true 
 * @return false 
 */
bool byteEndianess(){
    return byteLittleEndian;
}