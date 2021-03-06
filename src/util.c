#include "util.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  printf("%s", str);
  if (len > 0)
  {
    for (; len > 0; len--)
    {
      putchar(' ');
    }
  }
}

void fixPrintR(char const* str, int len)
{
  len -= strlen(str);
  if (len > 0)
  {
    for (; len > 0; len--)
    {
      putchar(' ');
    }
    printf("%s", str);
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
  printf("%s", str);
}

size_t arrayRemove(void* tab, size_t elemSize, size_t tabLen, size_t idx)
{
  // nothing to do if the element is the last one
  if (idx != tabLen - 1)
  {
    char* ptr = (char*)tab;
    for (size_t i = idx * elemSize; i < (tabLen - 1) * elemSize; i += elemSize)
    {
      memcpy(&ptr[i], &ptr[i + elemSize], elemSize);
    }
  }
  return tabLen - 1;
}

void printBytes(unsigned char const* bytes, int nb)
{
  for (int i = 0; i < nb; i++)
  {
    if (isprint(bytes[i]))
    {
      putchar(bytes[i]);
    }
    else
    {
      putchar('.');
    }
  }
}
