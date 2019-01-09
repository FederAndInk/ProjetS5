#include "ElfWriter.h"
#include "ElfIO.h"
#include "ElfParser.h"
#include "ElfString.h"
#include "ElfStringTable.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ndxUpdate(ElfImageP elfI, Elf32_Half minNdx)
{
  for (size_t k = 0; k < elfI->symbols.size; k++)
  {
    if (elfI->symbols.tab[k].st_shndx > minNdx)
    {
      elfI->symbols.tab[k].st_shndx--;
    }
  }

  for (size_t k = 0; k < elfI->sections.size; k++)
  {
    if (elfI->sections.tab[k].sh_link > minNdx)
    {
      elfI->sections.tab[k].sh_link--;
    }
  }
}

void deleteRelocationSections(ElfImageP elfI)
{
  for (size_t i = 0; i < elfI->sections.size; i++)
  {
    if (elfI->sections.tab[i].sh_type == SHT_REL ||
        elfI->sections.tab[i].sh_type == SHT_RELA)
    {
      elfI->sections.size = arrayRemove(elfI->sections.tab, sizeof(*elfI->sections.tab),
                                        elfI->sections.size, i);
      ndxUpdate(elfI, i);
    }

    // update e_shstrndx
    if (elfI->sections.tab[i].sh_type == SHT_STRTAB &&
        strcmp(".shstrtab", getSectionString(elfI, i)) == 0)
    {
      elfI->hdr.e_shstrndx = i;
    }
  }
  elfI->hdr.e_shnum = elfI->sections.size;
}

void writeSections(ElfImageP elfI, ElfFile dest, ElfFile src)
{
  for (size_t i = 0; i < elfI->sections.size; i++)
  {
    elfGoTo(src, elfI->sections.tab[i].sh_offset);
    elfI->sections.tab[i].sh_offset = elfTell(dest); // set new offset

    unsigned char* buf = elfReadUC_s(src, elfTell(src), elfI->sections.tab[i].sh_size);
    elfWriteUC_s(dest, elfTell(dest), elfI->sections.tab[i].sh_size, buf);
    free(buf);
    // section copied
  }
}

void writeSymbols(ElfImageP elfI, ElfFile dest) {
  // TODO: elfGoTo(offset of symbols)
}

void writeSectionHeaders(ElfImageP elfI, ElfFile dest)
{
  elfI->hdr.e_shoff = elfTell(dest);
  for (Elf32_Word i = 0; i < elfI->sections.size; i++)
  {
    elfWrite32(dest, elfI->sections.tab[i].sh_name);
    elfWrite32(dest, elfI->sections.tab[i].sh_type);
    elfWrite32(dest, elfI->sections.tab[i].sh_flags);
    elfWrite32(dest, elfI->sections.tab[i].sh_addr);
    elfWrite32(dest, elfI->sections.tab[i].sh_offset);
    elfWrite32(dest, elfI->sections.tab[i].sh_size);
    elfWrite32(dest, elfI->sections.tab[i].sh_link);
    elfWrite32(dest, elfI->sections.tab[i].sh_info);
    elfWrite32(dest, elfI->sections.tab[i].sh_addralign);
    elfWrite32(dest, elfI->sections.tab[i].sh_entsize);
  }
}

void setNewOffsets(ElfImageConstP elfI, ElfFile dest)
{
  // update e_type
  elfGoTo(dest, 0x10);
  // TODO: set new type

  // update e_shoff
  elfGoTo(dest, 0x20);
  elfWrite32(dest, elfI->hdr.e_shoff);

  // update e_shnum
  elfGoTo(dest, 0x30);
  elfWrite16(dest, elfI->hdr.e_shnum);

  // update e_shstrndx
  elfGoTo(dest, 0x32);
  elfWrite16(dest, elfI->hdr.e_shstrndx);
}