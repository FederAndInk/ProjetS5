#include "ElfWriter.h"
#include <stdlib.h>

void showHelp(char const* progName)
{
  printf("Usage: %s src dst\n", progName);
  printf("Options:\n");
  printf("  --\n");
}

int main(int argc, char* argv[])
{
  if (argc <= 3)
  {
    showHelp(argv[0]);
    exit(1);
  }
}