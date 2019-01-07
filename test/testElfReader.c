#include "ElfReader.h"
#include "UnitTest.h"
#include <elf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testIsElf(char const* fStr, bool hasToBeElf)
{
  // SEE: On declare notre test
  DECLARE_TEST("hasToBeElf: %s", hasToBeElf ? "true" : "false");

  bool isElfTmp;
  Elf  e = elfOpen(fStr);
  isElfTmp = isElf(e);
  check(isElf(e) == hasToBeElf, "%s has to be %s file.", fStr,
        hasToBeElf ? "an Elf" : "a non Elf");

  if (isElfTmp)
  {
    elfClose(e);
  }

  FILE* f = fopen(fStr, "rb");

  char   a[5] = {'\0'};
  size_t nbRead = fread(a, sizeof(*a), 4, f);
  if (isElfTmp)
  {
    if (check((nbRead == 4), "%s should have at least 4 characters to be an Elf file.",
              fStr))
    {
      check(a[0] == ELFMAG0, "First byte of %s should be MAG0", fStr);
      check(a[1] == ELFMAG1, "Second byte of %s should be MAG1", fStr);
      check(a[2] == ELFMAG2, "Third byte of %s should be MAG2", fStr);
      check(a[3] == ELFMAG3, "Fourth byte of %s should be MAG3", fStr);
    }
  }
  else
  {
    check((nbRead != 4) || (strcmp(a, ELFMAG) != 0),
          "%s was expected to be non Elf but it matches Elf format.", fStr);
  }
  fclose(f);
}

void testElfRead32(const char* f, Elf32_Word expected_vers, bool elfIsLE)
{
  /*expected_vers is the expected version of the file, which should always be 1*/
  DECLARE_TEST("expected_vers: %u", expected_vers);
  Elf file_elf = elfOpen(f);
  elfGoTo(file_elf, 14);
  Elf32_Word mot = elfRead32(file_elf);
  check(ftell(file_elf->f) == 18, "not well-placed cursor");
  check(expected_vers == mot, "didn't read correctly");
}

void testElfRead16(const char* f, Elf32_Half expected_type, bool elfIsLE)
{
  DECLARE_TEST("expected_type: %u", expected_type);
  Elf file_elf = elfOpen(f);
  elfGoTo(file_elf, 10);
  Elf32_Half mot = elfRead16(file_elf);
  check(ftell(file_elf->f) == 12, "not well-placed cursor");
  check(expected_type == mot, "didn't read correctly");
}

/*int testHeader(char const* f)
{

  check(ftell(e->f) == 0,
        "The position of the cursor should be at the begining of the file");

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

  // SEE: On encadre nos tests comme ça
  BEGIN_TESTS("TestElfReader");
  testIsElf(argv[1], true);
  testIsElf(argv[2], true);
  testIsElf(argv[3], false);
  testElfRead32(argv[1], 1, true);
  testElfRead32(argv[2], 1, false);
  testElfRead16(argv[1], 1, true);
  testElfRead16(argv[2], 1, false);
  // TODO: add more tests
  END_TESTS();
}