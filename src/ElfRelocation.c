#include "Elf.h"
#include "ElfParser.h"
#include "ElfReader.h"
#include "ElfString.h"
#include "ElfStringTable.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void showRelocationTable(ElfImageP elfI)
{

  if (elfI->rels.size == 0)
  {
    printf("\nThere are no relocations in this file.\n");
  }

  for (size_t i = 0; i < elfI->rels.size; i++)
  {

    printf("\nRelocation section '%s' at offset 0x%x contains %i entries:\n",
           getSectionString(elfI, elfI->rels.tab[i].sectionIdx),
           elfI->sections.tab[elfI->rels.tab[i].sectionIdx].sh_offset,
           elfI->rels.tab[i].nbRel);

    if (elfI->rels.tab[i].relType == SHT_RELA)
    {
      printf(" Offset     Info    Type            Sym.Value  Sym. Name + addend\n");
    }
    else
    {
      printf(" Offset     Info    Type            Sym.Value  Sym. Name\n");
    }

    for (size_t j = 0; j < elfI->rels.tab[i].nbRel; j++)
    {
      //offset
      printf("%.8x  ", elfI->rels.tab[i].rela[j].r_offset);
      //info
      printf("%.8x ", elfI->rels.tab[i].rela[j].r_info);
      //type
      Elf32_Word info = elfI->rels.tab[i].rela[j].r_info;
      fixPrint(getElfType(r_code, ELF32_R_TYPE(info)), 17);
      //valSym
      int indexTableSym = ELF32_R_SYM(info);
      printf(" %.8x", elfI->symbols.tab[indexTableSym].st_value);
      //name
      switch (elfI->symbols.tab[i].st_shndx)
      {
      case SHN_UNDEF:
      case SHN_LOPROC:
      case SHN_HIPROC:
      case SHN_ABS:
      case SHN_COMMON:
      case SHN_HIRESERVE:
      default:
        if (strlen(getSymbolString(elfI,
                                   ELF32_R_SYM(elfI->rels.tab[i].rela[j].r_info))) == 0)
        {
          printf(
              "   %s",
              getSectionString(
                  elfI, elfI->symbols.tab[ELF32_R_SYM(elfI->rels.tab[i].rela[j].r_info)]
                            .st_shndx));
        }
        else
        {
          printf("   %s",
                 getSymbolString(elfI, ELF32_R_SYM(elfI->rels.tab[i].rela[j].r_info)));
        }
        // addend handle
        if (elfI->rels.tab[i].relType == SHT_RELA)
        {
          printf(" + %.8x\n", elfI->rels.tab[i].rela[j].r_addend);
        }
        printf("\n");
        break;
      }
    }
  }
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
      parseHeader(elfIp, e);
      parseSectionHeaders(elfIp, e);
      parseStringTable(elfIp, e);

      parseSymboleTable(elfIp, e);

      parseRelocations(elfIp, e);

      showRelocationTable(elfIp);

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
