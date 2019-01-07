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
    elfWriteUC(dest, elfReadUC(input));
    elfI->sections.tab[i].sh_offset = elfTell(dest); // set new offset
    for (size_t k = 1; k < elfI->sections.tab[i].sh_size; k++)
    {
      elfWriteUC(dest, elfReadUC(input));
    }
    // section copied
  }
}

void writeElfHeaderInFile(ElfImageP elfI, Elf dest, Elf input)
{

    elfGoTo(input, 0);
    elfWriteUC(dest, elfReadUC(input));
    for (size_t i = 1; i < elfI->hdr.e_ehsize; i++)
    {
      elfWriteUC(dest, elfReadUC(input));
    }
}

void writeSSH(ElfImageP elfI, Elf dest, Elf input) {}
