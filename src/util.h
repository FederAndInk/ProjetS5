#pragma once
#include <stddef.h>
#include <stdio.h>

int is_big_endian();

void reverseEndianess(void* ptr, size_t size);

void fixPrint(char const* str, int len);

void fixPrintR(char const* str, int len);

void fixPrintaddr(char const* str, int len);
