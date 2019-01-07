#include "util.h"
#include <assert.h>
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

void arrayRemove(void* tab, size_t elemSize, size_t tabLen, size_t idx)
{

     if(idx < 0 || idx > tabLen)
    {
        printf("Invalid position! Please use a position between 0 to %d", tabLen-1);
    }
    else if (idx+elemSize > tabLen)
    {
        printf("The size of the element is to big");
    }
    else
    {
        char * ptr = (char *) tab;
        for(int i=(idx-1)+elemSize; i<tabLen-1; i++)
        {
             ptr[i] = ptr[i + 1];
        }
        realloc(tab,tabLen-elemSize);
    }

}
