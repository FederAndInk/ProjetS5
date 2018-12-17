#include "UnitTest.h"
#include <assert.h>
#include <stdio.h>

bool checksResults = true;

void check(bool cond, char const* expl)
{
  if (!cond)
  {
    fflush(stdout);
    fputs("\e[31m", stderr);
    fputs(expl, stderr);
    fputs("\e[1m", stderr);
    fputc('\n', stderr);
    fflush(stderr);
  }
  checksResults = cond && checksResults;
}

bool checksPassed()
{
  return checksResults;
}
