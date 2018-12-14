#include "ElfParser.h"
#include "ElfReader.h"
#include <linux/elf.h>
#include <stdio.h>

void showHeader(Elf32_Ehdr hdr)
{
  printf("ELF Header:\n");
  printf("  Magic:   7f 45 4c 46 01 01 01 00 00 00 00 00 00 00 00 00\n");
  printf("  Class:                             ELF32\n");
  printf("  Data:                              2's complement, little endian\n");
  printf("  Version:                           1 (current)\n");
  printf("  OS/ABI:                            UNIX - System V\n");
  printf("  ABI Version:                       0\n");
  printf("  Type:                              REL (Relocatable file)\n");
  printf("  Machine:                           ARM\n");
  printf("  Version:                           0x1\n");
  printf("  Entry point address:               0x0\n");
  printf("  Start of program headers:          0 (bytes into file)\n");
  printf("  Start of section headers:          460 (bytes into file)\n");
  printf("  Flags:                             0x5000000, Version5 EABI\n");
  printf("  Size of this header:               52 (bytes)\n");
  printf("  Size of program headers:           0 (bytes)\n");
  printf("  Number of program headers:         0\n");
  printf("  Size of section headers:           40 (bytes)\n");
  printf("  Number of section headers:         11\n");
  printf("  Section header string table index: 10\n");
}

int main(int argc, char* argv[])
{
  // TODO: parse header and show it

  if (argc != 2)
  {
    printf("Usage: %s ELF_File\n", argv[0]);
    return 1;
  }

  if (!isElf(argv[1]))
  {
    fprintf(stderr, "not an ELF file\n");
    return 1;
  }

  Elf e = elfOpen(argv[1]);

  Elf32_Ehdr hdr;
  parseHeader(e, &hdr);

  showHeader(hdr);
}