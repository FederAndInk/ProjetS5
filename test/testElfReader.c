#include "ElfIO.h"
#include "UnitTest.h"
#include <elf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testIsElf(char const* fStr, bool hasToBeElf, bool elfIsLE)
{
  // SEE: On declare notre test
  DECLARE_TEST("hasToBeElf: %s\n"
               "Little endian: %s",
               hasToBeElf ? "true" : "false", elfIsLE ? "yes" : "no");

  bool    isElfTmp;
  ElfFile e = elfOpen(fStr);
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

void testElfRead32(const char* f, Elf32_Word e_version, bool elfIsLE)
{
  /*e_version is the expected version of the file, which should always be 1 - byte 0x14*/
  DECLARE_TEST("e_version: %u\n"
               "Little endian: %s",
               e_version, elfIsLE ? "yes" : "no");
  ElfFile file_elf = elfOpen(f);
  if (check(isElf(file_elf), "File %s is not an Elf file", f))
  {
    elfGoTo(file_elf, 0x14);
    Elf32_Word mot = elfRead32(file_elf);
    check(elfTell(file_elf) == 0x18, "Misplaced cursor at %ld instead of %d",
          elfTell(file_elf), 0x18);
    check(e_version == mot, "Read %u instead of %u (e_version)", mot, e_version);
    elfClose(file_elf);
  }
}
void testElfRead16(const char* f, Elf32_Half e_type, bool elfIsLE)
{
  /* e_type is the expected type of the file, 0x01 for ET_REL - Byte 0x10 */
  DECLARE_TEST("e_type: %u\n"
               "Little endian: %s",
               e_type, elfIsLE ? "yes" : "no");
  ElfFile file_elf = elfOpen(f);

  if (check(isElf(file_elf), "File %s is not an Elf file", f))
  {
    elfGoTo(file_elf, 0x10);
    Elf32_Half mot = elfRead16(file_elf);
    check(elfTell(file_elf) == 0x10 + 0x2, "Misplaced cursor at %ld instead of %d",
          elfTell(file_elf), 0x12);
    check(e_type == mot, "Read %u instead of %u (e_type)", mot, e_type);
    elfClose(file_elf);
  }
}

void testElfReadUC(const char* f, unsigned char ei_version, bool elfIsLE)
{
  /* ei_version is the expected version of the file, 0x01 for current version - Byte 0x06 */
  DECLARE_TEST("ei_version: %u\n"
               "Little endian: %s",
               ei_version, elfIsLE ? "yes" : "no");
  ElfFile file_elf = elfOpen(f);

  if (check(isElf(file_elf), "File %s is not an Elf file", f))
  {
    elfGoTo(file_elf, 0x06);
    unsigned char mot = elfReadUC(file_elf);
    check(elfTell(file_elf) == 0x06 + 0x01, "Misplaced cursor at %ld instead of %d",
          elfTell(file_elf), 0x07);
    check(ei_version == mot, "Read %u instead of %u (ei_version)", mot, ei_version);
    elfClose(file_elf);
  }
}

void testElfReadUC_s(const char* f, size_t offset, size_t size, unsigned char* e_read,
                     bool elfIsLE)
{
  /* e_read is the expected chunk of data to read */
  DECLARE_TEST("Little endian : %s", elfIsLE ? "yes" : "no");
  ElfFile file_elf = elfOpen(f);
  if (check(isElf(file_elf), "File %s is not an Elf file", f))
  {
    unsigned char* str = elfReadUC_s(file_elf, offset, size);

    check(elfTell(file_elf) == offset + size, "Misplaced cursor at %ld instead of %d",
          elfTell(file_elf), offset + size);

    check(strncmp((char*)str, (char*)e_read, size) == 0,
          "Didn't read the expected value : %u %u %u instead of %u %u %u", str[0], str[1],
          str[2], e_read[0], e_read[1], e_read[2]);
    free(str);
    elfClose(file_elf);
  }
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
    fprintf(stderr, "Usage : %s LE_ElfV1File BE_ElfV1File nonElfFile\n", argv[0]);
    return 1;
  }

  const char* elfLE = argv[1];
  const char* elfBE = argv[2];
  const char* nonElf = argv[3];

  // SEE: On encadre nos tests comme ça
  BEGIN_TESTS("TestElfReader");
  testIsElf(elfLE, true, true);
  testIsElf(elfBE, true, false);
  testIsElf(nonElf, false, false);

  testElfRead32(elfLE, EV_CURRENT, true);
  testElfRead32(elfBE, EV_CURRENT, false);

  testElfRead16(elfLE, ET_REL, true);
  testElfRead16(elfBE, ET_REL, false);

  testElfReadUC(elfLE, EV_CURRENT, true);
  testElfReadUC(elfBE, EV_CURRENT, false);

  unsigned char e_read_1[] = {ELFCLASS32, ELFDATA2LSB, EV_CURRENT};
  testElfReadUC_s(elfLE, 0x04, 3, e_read_1, true);

  unsigned char e_read_2[] = {ELFCLASS32, ELFDATA2MSB, EV_CURRENT};
  testElfReadUC_s(elfBE, 0x04, 3, e_read_2, false);

  // TODO: add more tests
  END_TESTS();
}