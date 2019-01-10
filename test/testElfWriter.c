#include "ElfIO.h"
#include "UnitTest.h"
#include "util.h"
#include <elf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testElfWriteUC(const char* f, bool elfIsLe)
{
  DECLARE_TEST("Little endian : %s", elfIsLe ? "yes" : "no");
  ElfFile       file_elf = elfOpen(f);
  unsigned char result;

  if (check(isElf(file_elf), "%s is not an Elf file.", f))
  {
    elfGoTo(file_elf, 0x28);
    long pos = elfTell(file_elf);

    elfWriteUC(file_elf, 1);

    elfGoTo(file_elf, pos);
    fread(&result, 1, 1, file_elf->f);

    check(1 == result, "Wrong word written in file %s : %u instead of 1", f, result);

    elfClose(file_elf);
  }
}

void testElfWrite16(const char* f, bool elfIsLe)
{
  DECLARE_TEST("Little endian : %s", elfIsLe ? "yes" : "no");
  ElfFile       file_elf = elfOpen(f);
  unsigned char result[2];

  if (isElf(file_elf))
  {
    elfGoTo(file_elf, 0x28);
    long pos = elfTell(file_elf);
    elfWrite16(file_elf, 1);
    elfGoTo(file_elf, pos);
    fread(result, 2, 1, file_elf->f);

    if (elfIsLe)
    {
      check((result[0] == 1) && (result[1] == 0),
            "Wrong word written in file %s : %u %u instead of 1 0", f, result[0],
            result[1]);
    }
    else
    {
      check(result[0] == 0 && result[1] == 1,
            "Wrong word written in file %s : %u %u instead of 0 1", f, result[0],
            result[1]);
    }
    elfClose(file_elf);
  }
}
void testElfWrite32(const char* f, bool elfIsLe)
{
  DECLARE_TEST("Little endian : %s", elfIsLe ? "yes" : "no");
  ElfFile       file_elf = elfOpen(f);
  unsigned char result[4];

  if (check(isElf(file_elf), "%s is not an Elf file.", f))
  {
    elfGoTo(file_elf, 0x28);
    long pos = elfTell(file_elf);
    elfWrite32(file_elf, 1);
    elfGoTo(file_elf, pos);
    fread(result, 4, 1, file_elf->f);

    if (elfIsLe)
    {
      check((result[0] == 1) && (result[1] == 0) && (result[2] == 0) && (result[3] == 0),
            "Wrong word written in file %s : %u %u %u %u instead of 1 0 0 0", f,
            result[0], result[1], result[2], result[3]);
    }
    else
    {
      check((result[0] == 0) && (result[1] == 0) && (result[2] == 0) && (result[3] == 1),
            "Wrong word written in file %s : %u %u %u %u instead of 0 0 0 1", f,
            result[0], result[1], result[2], result[3]);
    }
    elfClose(file_elf);
  }
}

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

  testElfWrite16(elfLE, true);
  testElfWrite16(elfBE, false);

  testElfWrite32(elfLE, true);
  testElfWrite32(elfBE, false);

  testElfWriteUC(elfLE, true);
  testElfWriteUC(elfBE, false);

  // TODO: add more tests
  END_TESTS();
  return 0;
}
