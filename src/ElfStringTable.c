#include "ElfStringTable.h"
#include <stdlib.h>
#include <string.h>

char* getSectionString(ElfImageP elfI, Elf32_Word sectionNo)
{
  return (char*)&elfI->strTable.secStrs[elfI->sections.tab[sectionNo].sh_name];
}

char* getSymbolString(ElfImageP elfI, Elf32_Word symbolIdx)
{
  return (char*)&elfI->strTable.symStrs[elfI->symbols.tab[symbolIdx].st_name];
}

char* getSymbolName(ElfImageP elfI, Elf32_Word symbolIdx)
{
  Elf32_Sym* sym = &elfI->symbols.tab[symbolIdx];

  if (ELF32_ST_TYPE(sym->st_info) == STT_SECTION)
  {
    return getSectionString(elfI, sym->st_shndx);
  }
  else
  {
    return getSymbolString(elfI, symbolIdx);
  }
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
