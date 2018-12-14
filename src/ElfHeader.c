#include "ElfParser.h"
#include "ElfReader.h"
#include <linux/elf.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
  // TODO: parse header and show it

  if (argc != 2)
  {
    printf("Usage: %s ELF_File", argv[0]);
    return 1;
  }

  Elf e = elfOpen(argv[1]);

  Elf32_Ehdr hdr;
  parseHeader(e, hdr);
}