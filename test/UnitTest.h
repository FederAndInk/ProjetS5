#pragma once
#include <stdbool.h>

bool check(bool cond, char const* expl, ...);

void beginTests();
void endTests();

void addTest(char const* testName);
void finishTest(char const* testName);

bool checksPassed();
bool testsPassed();

#define BEGIN_TESTS(name) beginTests(name)

/**
 * @brief end the tests and return 0 if all tests passed or 1 if tests failed
 * 
 */
#define END_TESTS()                                                                      \
  endTests();                                                                            \
  if (testsPassed())                                                                     \
  {                                                                                      \
    return 0;                                                                            \
  }                                                                                      \
  else                                                                                   \
  {                                                                                      \
    return 1;                                                                            \
  }

#define DECLARE_TEST() addTest(__func__)
