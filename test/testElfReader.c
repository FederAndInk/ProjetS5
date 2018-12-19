#include "ElfReader.h"
#include "UnitTest.h"
#include <elf.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{

  if (argc != 4)
  {
    fprintf(stderr, "Usage : %s LEElfFile BEElfFile nonElfFil\n", argv[0]);
    return 1;
  }


  Elf e;

  for (int i = 1; i < argc; i++)
  {

    printf("===> test on: %s\n", argv[i]);
    fflush(stdout);

    e = elfOpen(argv[i]);
    if (!isElf(e))
    {
      fprintf(stderr, "(isElf): %s isn't an ELF file !\n", argv[i]);
      return 1;
    }

    check(ftell(e->f) == 0,
          "The position of the cursor should by at the begining of the file");

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
  }

  // TODO: add more tests

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