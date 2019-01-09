#include "ElfStringTable.h"
#include <stdlib.h>
#include <string.h>

char const* getSectionString(ElfImageConstP elfI, Elf32_Word sectionNo)
{
  return (char*)&elfI->strTable.secStrs[elfI->sections.tab[sectionNo].sh_name];
}

char const* getSymbolString(ElfImageConstP elfI, Elf32_Word symbolIdx)
{
  return (char*)&elfI->strTable.symStrs[elfI->symbols.tab[symbolIdx].st_name];
}

char const* getSymbolName(ElfImageConstP elfI, Elf32_Word symbolIdx)
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

typedef char const* (*GetStringFn)(ElfImageConstP elfI, Elf32_Word idx);

Elf32_Word getIdFromStr(char const* str, Elf32_Word tabSize, GetStringFn getString,
                        ElfImageConstP elfI)
{
  Elf32_Word i;

  char* strEndParse;
  i = strtol(str, &strEndParse, 10);

  // strEndParse points to the char after the last char parsed by strtol
  // and is equal to name if it can't be parsed
  if (str == strEndParse)
  {
    i = 0;
    while (i < tabSize && strcmp(getString(elfI, i), str) != 0)
    {
      i++;
    }
  }

  if (i >= tabSize)
  {
    i = tabSize;
  }

  return i;
}

Elf32_Word getSectionIdFromStr(ElfImageConstP elfI, char const* str)
{
  return getIdFromStr(str, elfI->sections.size, getSectionString, elfI);
}

Elf32_Word getSymbolIdFromStr(ElfImageConstP elfI, char const* str)
{
  return getIdFromStr(str, elfI->symbols.size, getSymbolString, elfI);
}
