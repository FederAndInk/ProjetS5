#include "ElfStringTable.h"
#include <stdlib.h>
#include <string.h>

char* getSectionString(ElfImageP elfI, Elf32_Word sectionNo)
{
  return (char*)&elfI->strTable.secStrs[elfI->sections.tab[sectionNo].sh_name];
}

char* getSymbolString(ElfImageP elfI, Elf32_Word offIntoTab)
{
  return (char*)&elfI->strTable.symStrs[offIntoTab];
}

Elf32_Word getSectionIdFromStr(ElfImageP elfI, char const* name)
{
  Elf32_Word i = 0;
  while (i < elfI->sections.size && strcmp((char*)getSectionString(elfI, i), name) != 0)
  {
    i++;
  }

  return i;
}
