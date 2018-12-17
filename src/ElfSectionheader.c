#include "ElfParser.h"
#include "ElfReader.h"
#include <linux/elf.h>
#include <stdio.h>

void showSectionHeader(Elf32_Ehdr* tabshdr[],ElfN_Off shoff,uint16_t shnum)
{
    printf("There are %d section headers, starting at offset 0x%d:",shnum,shoff);

    /*
There are 7 section headers, starting at offset 0x40:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .data             PROGBITS         0000000000000000  00000200
       000000000000000d  0000000000000000  WA       0     0     4
  [ 2] .text             PROGBITS         0000000000000000  00000210
       0000000000000027  0000000000000000  AX       0     0     16
  [ 3] .shstrtab         STRTAB           0000000000000000  00000240
       0000000000000032  0000000000000000           0     0     1
  [ 4] .symtab           SYMTAB           0000000000000000  00000280
       00000000000000a8  0000000000000018           5     6     4
  [ 5] .strtab           STRTAB           0000000000000000  00000330
       0000000000000034  0000000000000000           0     0     1
  [ 6] .rela.text        RELA             0000000000000000  00000370
       0000000000000018  0000000000000018           4     2     4
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), l (large)
  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
  O (extra OS processing required) o (OS specific), p (processor specific)
*/
}

int main(int argc, char* argv[])
{
  // TODO: parse header and show it

  if (argc != 2)
  {
    printf("Usage: %s ELF_File\n", argv[0]);
    return 1;
  }

  Elf e = elfOpen(argv[1]);

  if (!isElf(e))
  {
    fprintf(stderr,
            "%s: Error: Not an ELF file - it has the wrong magic bytes at the start\n",
            argv[0]);
    return 1;
  }
  else
  {
    Elf32_Ehdr hdr;
    parseHeader(e, &hdr);
    if (hdr.e_ident[EI_CLASS] == ELFCLASS32)
    {
      Elf32_Shdr tabshdr[hdr.e_shnum];
      parseSectionHeader(e, &tabshdr,hdr.e_shoff,hdr.e_shnum);
      showSectionHeader(&tabshdr,hdr.e_shoff,hdr.e_shnum);
      return 0;
    }
    else
    {
      fprintf(stderr, "Unsupported class, parses only ELF32\n");
      return 1;
    }
  }
}
