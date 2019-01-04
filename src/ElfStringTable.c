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

Elf32_Word getSectionIdFromStr(ElfImageP elfI, char const* str)
{
  Elf32_Word i;

  char* strEndParse;
  i = strtol(str, &strEndParse, 10);

  // strEndParse points to the char after the last char parsed by strtol
  // and is equal to name if it can't be parsed
  if (str == strEndParse)
  {
    i = 0;
    while (i < elfI->sections.size && strcmp((char*)getSectionString(elfI, i), str) != 0)
    {
      i++;
    }
  }

  if (i < 0 || i >= elfI->sections.size)
  {
    i = elfI->sections.size;
  }

  return i;
}
