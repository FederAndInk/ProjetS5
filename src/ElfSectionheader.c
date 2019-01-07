#include "Elf.h"
#include "ElfParser.h"
#include "ElfReader.h"
#include "ElfString.h"
#include "ElfStringTable.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void showSectionHeader(ElfImageP elfI)
{
  printf("There are %d section headers, starting at offset 0x%x:\n", elfI->hdr.e_shnum,
         elfI->hdr.e_shoff);
  printf("Section Headers:\n");
  printf("Nr  Name                Type             Address  Offset Size   ES Flags Link  "
         "Info  Align\n");
  for (int i = 0; i < elfI->hdr.e_shnum; i++)
  {
    char id[4];
    sprintf(id, "%d", i);
    fixPrint(id, 4);

    char* str = (char*)getSectionString(elfI, i);
    fixPrint(str, 20);

    char const* strType = getElfType(sht, elfI->sections.tab[i].sh_type);
    fixPrint(strType, 17);

    printf("%.8x %.6x %.6x %.2x ", elfI->sections.tab[i].sh_addr,
           elfI->sections.tab[i].sh_offset, elfI->sections.tab[i].sh_size,
           elfI->sections.tab[i].sh_entsize);

    int x = 0;

    if (elfI->sections.tab[i].sh_flags & SHF_WRITE)
    {
      printf("W");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_ALLOC)
    {
      printf("A");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_EXECINSTR)
    {
      printf("X");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_MERGE)
    {
      printf("M");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_STRINGS)
    {
      printf("S");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_INFO_LINK)
    {
      printf("I");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_LINK_ORDER)
    {
      printf("L");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_GROUP)
    {
      printf("G");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_TLS)
    {
      printf("T");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_MASKOS)
    {
      printf("o");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_MASKPROC)
    {
      printf("p");
      x++;
    }

    for (int j = 0; j < (5 - x); j++)
      putchar(' ');

    char lnk[16];
    printf(" ");
    sprintf(lnk, "%d", elfI->sections.tab[i].sh_link);
    fixPrint(lnk, 5);

    char info[16];
    printf(" ");
    sprintf(info, "%d", elfI->sections.tab[i].sh_info);
    fixPrint(info, 6);

    printf("%d \n", elfI->sections.tab[i].sh_addralign);
  }
  printf("\nKey to Flags:\n");
  printf("W (write), A (alloc), X (execute), M (merge), S (strings), l (large)\n");
  printf("I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
  printf("O (extra OS processing required) o (OS specific), p (processor specific)\n");
}

int main(int argc, char* argv[])
{

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
    ElfImage  elfI;
    ElfImageP elfIp = &elfI;
    initElfImage(elfIp);

    parseHeader(elfIp, e);
    if (elfI.hdr.e_ident[EI_CLASS] == ELFCLASS32)
    {
      parseSectionHeaders(elfIp, e);
      parseStringTable(elfIp, e);

      showSectionHeader(elfIp);

      deleteElfImage(elfIp);
      elfClose(e);
      return 0;
    }
    else
    {
      fprintf(stderr, "Unsupported class, parses only ELF32\n");

      deleteElfImage(elfIp);
      elfClose(e);
      return 1;
    }
  }
}