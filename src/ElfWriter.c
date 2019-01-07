#include "ElfWriter.h"
#include "ElfParser.h"
#include "ElfReader.h"
#include "ElfString.h"
#include "ElfStringTable.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ElfImageP deleteRelocationSections(ElfImageP elfI)
{
  for (size_t i = 0; i < elfI->sections.size; i++)
  {
    if (elfI->sections.tab[i].sh_type == SHT_REL ||
        elfI->sections.tab[i].sh_type == SHT_RELA)
    {
      arrayRemove(elfI->sections.tab, sizeof(*elfI->sections.tab), elfI->sections.size,
                  i);
      elfI->sections.size--;
    }
  }
  elfI->hdr.e_shnum = elfI->sections.size;
  return elfI;
}

void writeSections(ElfImageP elfI, Elf dest, Elf src)
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

void writeSectionHeaders(ElfImageP elfI, Elf dest)
{
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

void setNewOffsets(ElfImageP elfI, Elf dest)
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