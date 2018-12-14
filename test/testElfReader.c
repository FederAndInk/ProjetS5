#include "ElfReader.h"
#include <stdlib.h>

int main(int argc, char* argv[])
{
  printf("===> test on: %s\n", argv[1]);
  fflush(stdout);

  // TODO make tests with ElfReader

  if (!isElf(argv[1]))
  {
    fprintf(stderr, "(isElf): File given isn't an ELF file !\n");
    return 1;
  }

  // elfOpen(argv[1]);
}