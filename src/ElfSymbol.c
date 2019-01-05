#include "Elf.h"
#include "ElfParser.h"
#include "ElfReader.h"
#include "ElfString.h"
#include "ElfStringTable.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void showSymbolTable(ElfImageP elfI)
{
  printf("Symbol table '.symtab' contains %i entries:\n", elfI->symbols.size);
  printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");

  for (size_t i = 0; i < elfI->symbols.size; i++)
  {
    // num
    char strNum[7];
    snprintf(strNum, 6, "%ld:", i);
    fixPrintR(strNum, 7);
    // value
    printf(" %.8x ", elfI->symbols.tab[i].st_value);
    // length
    snprintf(strNum, 5, "%d", elfI->symbols.tab[i].st_size);
    fixPrintR(strNum, 5);
    // type
    unsigned char info = elfI->symbols.tab[i].st_info;
    printf("%s", getElfType(stt, ELF32_ST_TYPE(info)));
    //link
    printf("%s", getElfType(stb, ELF32_ST_BIND(info)));
    // range
    printf("%s", getElfType(stv, ELF32_ST_VISIBILITY(elfI->symbols.tab[i].st_other)));
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
      printf("%3i ", elfI->symbols.tab[i].st_shndx);
      break;
    }

    // name
    char* strName = getSymbolString(elfI, i);

    fixPrint(strName, 20);
    putchar('\n');
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

      parseSymbolTable(elfIp, e);

      showSymbolTable(elfIp);

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
