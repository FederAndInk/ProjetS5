#include "Elf.h"
#include "ElfParser.h"
#include "ElfReader.h"
#include "ElfString.h"
#include "ElfStringTable.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void showSymboleTable(ElfImageP elfI)
{
  printf("La table de symboles « .symtab » contient %i entrées", elfI->symbols.size);
  printf("   Num:    Valeur Tail Type    Lien   Vis      Ndx Nom\n");

  for (size_t i = 0; i < elfI->symbols.size; i++)
  {
    // num
    char* strNum[7];
    sprintf(strNum, "%i:", i);
    fixPrint(strNum, 7);
    // value
    printf(" %.8x:", elfI->symbols.tab[i].st_size);
    // length
    printf(" %.6x:", elfI->symbols.tab[i].st_size);
    // type
    unsigned char info = elfI->symbols.tab[i].st_info;
    printf(getElfType(stt, ELF32_ST_BIND(info)));
    //link
    printf(getElfType(stb, ELF32_ST_TYPE(info)));
    // range
    printf(getElfType(stv, ELF32_ST_VISIBILITY(elfI->symbols.tab[i].st_other)));
    // section index
    switch (elfI->symbols.tab[i].st_shndx)
    {
    case SHN_UNDEF:
      printf("UND ");
      break;
    case SHN_LOPROC:
      printf("LOP ");
      break;
    case SHN_HIPROC:
      printf("HIP ");
      break;
    case SHN_ABS:
      printf("ABS ");
      break;
    case SHN_COMMON:
      printf("COM ");
      break;
    case SHN_HIRESERVE:
      printf("HIR ");
      break;

    default:
      printf("%i", elfI->symbols.tab[i].st_shndx);
      break;
    }

    // name
    char* strName = getSymbolString(elfI, elfI->symbols.tab[i].st_name);
    fixPrint(strName, 20);
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
      parseSectionHeaders(elfIp, e);
      parseStringTable(elfIp, e);

      showSymboleTable(elfIp);

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
