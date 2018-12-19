#include "ElfParser.h"
#include "ElfReader.h"
#include "ElfString.h"
#include "util.h"
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>

void showSectionHeader(Elf32_Shdr tabshdr[],uint16_t shoff,uint16_t shnum,uint16_t stind,Elf e)
{
    printf("There are %d section headers, starting at offset 0x%x:\n",shnum,shoff);
    printf("Section Headers:\n");
    printf("Nr  Name              Type             Address  Offset Size   ES Flags Link  Info  Align\n");
    for (int i = 0 ; i<shnum ; i++)
    {
        char id[4];
        sprintf( id,  "%d", i );
        fixPrint(id,4);


        char* str=StrtableGetString(e,(tabshdr[stind].sh_offset+tabshdr[i].sh_name));
        fixPrint(str,20);
        free(str);

        char const * strType=getElfType(sht,tabshdr[i].sh_type);
        fixPrint(strType,17);

        printf("%.8x %.6x %.6x %.2x ",tabshdr[i].sh_addr,
               tabshdr[i].sh_offset,tabshdr[i].sh_size,tabshdr[i].sh_entsize);

        int x = 0;

        if (tabshdr[i].sh_flags & SHF_WRITE)
            {printf("W");x++;}
        if (tabshdr[i].sh_flags & SHF_ALLOC)
            {printf("A");x++;}
        if (tabshdr[i].sh_flags & SHF_EXECINSTR)
            {printf("X");x++;}
        if (tabshdr[i].sh_flags & SHF_MERGE)
            {printf("M");x++;}
        if (tabshdr[i].sh_flags & SHF_STRINGS)
            {printf("S");x++;}
        if (tabshdr[i].sh_flags & SHF_INFO_LINK)
            {printf("I");x++;}
        if (tabshdr[i].sh_flags & SHF_LINK_ORDER)
            {printf("L");x++;}
        if (tabshdr[i].sh_flags & SHF_GROUP)
            {printf("G");x++;}
        if (tabshdr[i].sh_flags & SHF_TLS)
            {printf("T");x++;}
        if (tabshdr[i].sh_flags & SHF_MASKOS)
            {printf("o");x++;}
        if (tabshdr[i].sh_flags & SHF_MASKPROC)
            {printf("p");x++;}


        for(int j=0;j<(5-x);j++)
            putchar(' ');

        char lnk[16];
        printf(" ");
        sprintf( lnk,  "%d", tabshdr[i].sh_link );
        fixPrint(lnk,5);

        char info[16];
        printf(" ");
        sprintf( info,  "%d", tabshdr[i].sh_info );
        fixPrint(info,6);

        printf("%d \n",tabshdr[i].sh_addralign);
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
    Elf32_Ehdr hdr;
    parseHeader(e, &hdr);
    if (hdr.e_ident[EI_CLASS] == ELFCLASS32)
    {
      Elf32_Shdr tabshdr[hdr.e_shnum];
      parseSectionHeader(e, tabshdr,hdr.e_shoff,hdr.e_shnum);
      showSectionHeader(tabshdr,hdr.e_shoff,hdr.e_shnum,hdr.e_shstrndx,e);
      return 0;
    }
    else
    {
      fprintf(stderr, "Unsupported class, parses only ELF32\n");
      return 1;
    }
  }
}
