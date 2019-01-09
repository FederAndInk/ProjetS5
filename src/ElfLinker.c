#include "ElfIO.h"
#include "ElfImage.h"
#include "ElfParser.h"
#include "ElfString.h"
#include "ElfStringTable.h"
#include "ElfWriter.h"
#include "elf.h"
#include "util.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void showHelp(char const* progName)
{
  printf("Usage: %s src dst\n", progName);
  printf("Options:\n");
  printf("  --\n");
}

void setSectionAddr(ElfImageP elfI, char const* name, Elf32_Addr addr)
{
  elfI->sections.tab[getSectionIdFromStr(elfI, name)].sh_addr = addr;
}

void setSymbolsAddr(ElfImageP elfi)
{
  for (size_t i = 0; i < elfi->symbols.size; i++)
  {
    elfi->symbols.tab[i].st_value +=
        elfi->sections.tab[elfi->symbols.tab[i].st_shndx].sh_addr;
  }
}

void getSectionNameAndAddress(char* str, char** sectionName, Elf32_Addr* sectionAddress)
{
  *sectionName = (char*)malloc(strlen(str) + 1);
  int i = 0;
  for (; str[i] != '\0' && str[i] != '='; i++)
  {
    (*sectionName)[i] = str[i];
  }
  (*sectionName)[i] = '\0';

  if (str[i] != '=')
  {
    printf("syntaxe incorrecte");
  }
  else
  {
    i++;
    *sectionAddress = strtol(str + i, NULL, 0);
  }
}

int main(int argc, char* argv[])
{
  if (argc <= 3)
  {
    showHelp(argv[0]);
    exit(1);
  }
  ElfFile   e = elfOpen(argv[1]);
  ElfImage  elfi;
  ElfImageP elfIp = &elfi;
  initElfImage(elfIp);
  if (parseElf(elfIp, e))
  {
    deleteRelocationSections(elfIp);
    for (int k = 3; k < argc; k++)
    {
      Elf32_Addr secAddr;
      char*      secName;
      getSectionNameAndAddress(argv[k], &secName, &secAddr);
      setSectionAddr(elfIp, secName, secAddr);
      free(secName);
    }
    setSymbolsAddr(elfIp);
    ElfFile dest = elfCreate(&elfIp->hdr, argv[2]);
    copySections(elfIp, dest, e);
    writeSymbols(elfIp, dest);
    writeSectionHeaders(elfIp, dest);

    // rewrite header to update offsets and ndx
    writeElfHeader(&elfIp->hdr, dest);
    elfClose(dest);
    deleteElfImage(elfIp);
  }
  else
  {
    printf("%s can't be parsed", argv[1]);
  }
  elfClose(e);
}