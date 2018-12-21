#include "ElfReader.h"
#include "UnitTest.h"
// #include <CUnit/Basic.h>
#include <CUnit/CUnitCI.h>
#include <elf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testIsElf(char const* fStr, bool hasToBeElf)
{
  bool isElfTmp;
  Elf  e = elfOpen(fStr);
  isElfTmp = isElf(e);
  check(isElf(e) == hasToBeElf, "(isElf): %s has to be %s file.", fStr,
        hasToBeElf ? "an Elf" : "a non Elf");

  if (e)
  {
    elfClose(e);
  }

  FILE* f = fopen(fStr, "rb");

  char   a[5] = {'\0'};
  size_t nbRead = fread(a, sizeof(*a), 4, f);
  if (isElfTmp)
  {
    if (check((nbRead == 4),
              "(isElf): %s should have at least 4 characters to be an Elf file.", fStr))
    {
      check(a[0] == ELFMAG0, "(isElf): First byte of %s should be MAG0", fStr);
      check(a[1] == ELFMAG1, "(isElf): Second byte of %s should be MAG1", fStr);
      check(a[2] == ELFMAG2, "(isElf): Third byte of %s should be MAG2", fStr);
      check(a[3] == ELFMAG3, "(isElf): Fourth byte of %s should be MAG3", fStr);
    }
  }
  else
  {
    check((nbRead != 4) || (strcmp(a, ELFMAG) != 0),
          "(isElf): %s was expected to be non Elf but it matches Elf format.", fStr);
  }

  fclose(f);
}

/*int testHeader(char const* f)
{
  printf("===> test on: %s\n", f);
  fflush(stdout);

  Elf e = elfOpen(f);
  if (!isElf(e))
  {
    fprintf(stderr, "(isElf): %s is not an ELF file !\n", f);
    return 1;
  }

  check(ftell(e->f) == 0,
        "The position of the cursor should be at the begining of the file");

  check(elfReadUC(e) == ELFMAG0, "first byte should be the MAG0");
  check(elfReadUC(e) == ELFMAG1, "second byte should be the MAG1");
  check(elfReadUC(e) == ELFMAG2, "third byte should be the MAG2");
  check(elfReadUC(e) == ELFMAG3, "fourth byte should be the MAG3");

  elfGoTo(e, 0);
  check(elfReadUC(e) == ELFMAG0, "first byte (after goto 0) should be the MAG0");
  elfGoTo(e, 0);
  elfWriteUC(e, 'T');
  elfGoTo(e, 0);
  check(elfReadUC(e) == 'T', "first byte (after writing 'T') should be 'T'");
  elfGoTo(e, 0);
  elfWriteUC(e, ELFMAG0);
  elfGoTo(e, 0);
  check(elfReadUC(e) == ELFMAG0,
        "first byte (after rewriting ELFMAG0) should be ELFMAG0");

  elfClose(e);
}*/

int main(int argc, char* argv[])
{

  if (argc != 4)
  {
    fprintf(stderr, "Usage : %s LE_ElfFile BE_ElfFile nonElfFile\n", argv[0]);
    return 1;
  }

  testIsElf(argv[1], true);
  testIsElf(argv[2], true);
  testIsElf(argv[3], false);

  // TODO: add more tests
  // Pour ex1.o : type

  // Pour ex1.o : type

  if (checksPassed())
  {
    return 0;
  }
  else
  {
    return 1;
  }
}