#pragma once
#include <stddef.h>

int is_big_endian();

void reverseEndianess(void* ptr, size_t size);
