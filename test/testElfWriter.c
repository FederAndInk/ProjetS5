#include "ElfIO.h"
#include "UnitTest.h"
#include <elf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testElfWriteUC(const char* f, unsigned char e, bool elfIsLe)
{
  DECLARE_TEST("Little endian : %s", elfIsLe ? "yes" : "no");
  ElfFile       file_elf = elfOpen(f);
  unsigned char result;

  if (isElf(file_elf))
  {
    elfWriteUC(file_elf, e);

    elfGoTo(file_elf, elfTell(file_elf) - 1);
    result = elfReadUC(file_elf);

    check(e == result, "Wrong word written in file %s : %u instead of %u", f, result, e);

    elfClose(file_elf);
  }
}

void testElfWrite16(const char* f, Elf32_Half e, bool elfIsLe)
{
  DECLARE_TEST("Little endian : %s", elfIsLe ? "yes" : "no");
  ElfFile    file_elf = elfOpen(f);
  Elf32_Half result;

  if (isElf(file_elf))
  {
    elfWrite16(file_elf, e);

    elfGoTo(file_elf, elfTell(file_elf) - 2);
    result = elfRead16(file_elf);

    check(e == result, "Wrong word written in file %s : %d instead of %d", f, result, e);

    elfClose(file_elf);
  }
}
void testElfWrite32(const char* f, Elf32_Word e, bool elfIsLe)
{
  DECLARE_TEST("Little endian : %s", elfIsLe ? "yes" : "no");
  ElfFile    file_elf = elfOpen(f);
  Elf32_Word result;

  if (isElf(file_elf))
  {
    elfWrite32(file_elf, e);

    elfGoTo(file_elf, elfTell(file_elf) - 4);
    result = elfRead32(file_elf);

    check(e == result, "Wrong word written in file %s : %d instead of %d", f, result, e);

    elfClose(file_elf);
  }
}

void elfWriteUC_s(ElfFile f, size_t offset, size_t size, unsigned char* buf) {}

int main(int argc, char const* argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "Usage : %s LE_ElfV1File BE_ElfV1File\n", argv[0]);
    return 1;
  }

  const char* elfLE = argv[1];
  const char* elfBE = argv[2];

  // SEE: On encadre nos tests comme ça
  BEGIN_TESTS("TestElfReader");

  // TODO: add more tests
  END_TESTS();
  return 0;
}
