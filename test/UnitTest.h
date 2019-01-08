#pragma once
#include <stdbool.h>

bool check(bool cond, char const* expl, ...);

void beginTests(char const* name);
void endTests();

void addTest(char const* testName, char const* format, ...);
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

/**
 * @brief declare a test with a description of the current test
 * use a format string
 * and may use args like printf
 */
#define DECLARE_TEST(formatArgs...) addTest(__func__, formatArgs)
