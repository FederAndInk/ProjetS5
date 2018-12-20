#include "UnitTest.h"
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>

bool checksResults = true;

bool check(bool cond, char const* expl, ...)
{
  if (!cond)
  {
    fflush(stdout);
    fputs("\e[31m", stderr);

    va_list list;
    va_start(list, expl);
    vfprintf(stderr, expl, list);
    va_end(list);
    fputs("\e[1m", stderr);
    fputc('\n', stderr);
    fflush(stderr);
  }
  checksResults = cond && checksResults;
  return cond;
}

bool checksPassed()
{
  return checksResults;
}
