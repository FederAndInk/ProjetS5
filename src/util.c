#include "util.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int is_big_endian()
{
  static uint32_t one = 1;
  return ((*(uint8_t*)&one) == 0);
}

void reverseEndianess(void* ptr, size_t size)
{
  assert(size <= 16);
  uint8_t* p = (uint8_t*)ptr;
  uint8_t  pTmp[16];
  memcpy(pTmp, p, size);

  for (size_t i = 0; i < size; i++)
  {
    p[i] = pTmp[size - i - 1];
  }
}

void fixPrint(char const* str, int len)
{
  len -= strlen(str);
  printf("%s",str);
  if (len > 0)
  {
    for (; len > 0; len--)
    {
      putchar(' ');
    }
  }

}


void fixPrintaddr(char const* str, int len)
{
  len -= strlen(str);
  if (len > 0)
  {
    for (; len > 0; len--)
    {
      putchar('0');
    }
  }
  printf("%s",str);
}
