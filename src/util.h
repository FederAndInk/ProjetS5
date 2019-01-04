#pragma once
#include <stddef.h>
#include <stdio.h>

int is_big_endian();

void reverseEndianess(void* ptr, size_t size);

void fixPrint(char const* str, int len);

void fixPrintaddr(char const* str, int len);

/**
 * @brief fancy print bytes
 * if printable (eg: graph) print the byte or print a dot '.' otherwise
 * 
 * @param bytes 
 * @param nb 
 */
void printBytes(unsigned char const* bytes, int nb);
