#include "Elf.h"
#include "ElfParser.h"
#include "ElfReader.h"
#include "ElfString.h"
#include "ElfStringTable.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ElfImageP deleteRelocationSectionsInElfI(ElfImageP elfI)
{
  int table[elfI->sections.size];
  for (size_t i = 0; i < elfI->sections.size; i++)
  {
    if (elfI->sections.tab[i].sh_type == SHT_REL ||
        elfI->sections.tab[i].sh_type == SHT_RELA)
    {
      arrayRemove(table, sizeof(int), elfI->sections.size, i);
    }
  }
  return elfI;
}

void writeSectionsInFile(ElfImageP elfI, Elf dest, Elf input)
{
  for (size_t i = 0; i < elfI->sections.size; i++)
  {
    elfGoTo(input, elfI->sections.tab[i].sh_offset);
    elfI->sections.tab[i].sh_offset = elfTell(dest); // set new offset
    for (size_t k = 0; k < elfI->sections.tab[i].sh_size; k++)
    {
      elfWriteUC(dest, elfReadUC(input));
    }
    // section copied
  }
}

void writeSectionHeaders(ElfImageP elfI, Elf dest, Elf input)
{
  elfI->sections.size = elfI->hdr.e_shnum;

  elfI->sections.tab = (Elf32_Shdr*)malloc(elfI->sections.size * sizeof(Elf32_Shdr));
  elfI->hdr.e_shoff = elfTell(dest);
  for (Elf32_Word i = 0; i < elfI->sections.size; i++)
  {
    elfWrite32(dest, elfI->sections.tab[i].sh_name);
    elfWrite32(dest, elfI->sections.tab[i].sh_type);
    elfWrite32(dest, elfI->sections.tab[i].sh_addr);
    elfWrite32(dest, elfI->sections.tab[i].sh_offset);
    elfWrite32(dest, elfI->sections.tab[i].sh_size);
    elfWrite32(dest, elfI->sections.tab[i].sh_link);
    elfWrite32(dest, elfI->sections.tab[i].sh_info);
    elfWrite32(dest, elfI->sections.tab[i].sh_addralign);
    elfWrite32(dest, elfI->sections.tab[i].sh_entsize);
  }
}

void modifyElfHeader(ElfImageP elfI, Elf dest)
{
  //modify e_phoff
  elfGoTo(dest, 0x1c);
  //TODO set e_phoff

  //modify e_shstrndx
  elfGoTo(dest, 0x30);
  //elfWrite16(dest, )

  //modify e_shnum
  elfGoTo(dest, 0x30);
  elfWrite16(dest, elfI->sections.size);
}