#include "UnitTest.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

size_t nbCheck = 0;
size_t nbCheckPassed = 0;

bool check(bool cond, char const* expl, ...)
{
  nbCheck++;
  if (!cond)
  {
    fflush(stdout);
    fputs("\e[31m", stderr);

    va_list list;
    va_start(list, expl);
    vfprintf(stderr, expl, list);
    va_end(list);
    fputs("\e[0m", stderr);
    fputc('\n', stderr);
    fflush(stderr);
  }
  else
  {
    nbCheckPassed++;
  }
  return cond;
}

size_t      nbTest = 0;
size_t      nbTestPassed = 0;
char const* currentTest = NULL;

void beginTests(char const* name)
{
  printf("Testing %s\n", name);
}

void endTests()
{
  if (currentTest)
  {
    finishTest(currentTest);

    puts("\e[1m<~~~~~~~~~~~~Result~~~~~~~~~~~~>\e[0m");
    printf("Tests finished\n");
    printf("\e[1;36m%ld/%ld\e[0m tests passed\n", nbTestPassed, nbTest);
    puts(">~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~<");

    if (testsPassed())
    {
      printf("\e[32;1m");
      puts("Overall tests passed");
      printf("\e[35;1mYay");
      printf("\e[0m");
    }
    else
    {
      printf("\e[31;1m");
      puts("Overall tests failed");
      printf("\e[34;1m:'(");
      printf("\e[0m");
    }
  }
  else
  {
    printf("No Tests !");
  }
  putchar('\n');
}

void addTest(char const* testName, char const* format, ...)
{
  if (currentTest)
  {
    finishTest(currentTest);
  }

  currentTest = testName;
  nbTest++;
  nbCheck = 0;
  nbCheckPassed = 0;
  printf("\e[35;4mLaunch %s\e[0m\n", testName);
  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  putchar('\n');
  puts("<========Program Output========>");
}

void finishTest(char const* testName)
{
  puts(">==============================<");
  printf("Test %s finished\n", testName);
  printf("\e[1;36m%ld/%ld\e[0m checks passed\n", nbCheckPassed, nbCheck);

  if (checksPassed())
  {
    nbTestPassed++;
    printf("\e[32m");
    printf("Test %s passed", testName);
    printf("\e[0m");
  }
  else
  {
    printf("\e[31m");
    printf("Test %s failed", testName);
    printf("\e[0m");
  }
  puts("\n");
}

bool checksPassed()
{
  return nbCheck == nbCheckPassed;
}

bool testsPassed()
{
  return nbTest == nbTestPassed;
}