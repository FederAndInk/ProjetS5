#include "ElfWriter.h"
#include "ElfParser.h"
#include "ElfReader.h"
#include "ElfString.h"
#include "ElfStringTable.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void deleteRelocationSections(ElfImageP elfI)
{
  // TODO: update e_shstrndx,

  for (size_t i = 0; i < elfI->sections.size; i++)
  {
    if (elfI->sections.tab[i].sh_type == SHT_REL ||
        elfI->sections.tab[i].sh_type == SHT_RELA)
    {
      elfI->sections.size = arrayRemove(elfI->sections.tab, sizeof(*elfI->sections.tab),
                                        elfI->sections.size, i);
    }
  }
  elfI->hdr.e_shnum = elfI->sections.size;
}

void writeElfHeader(ElfImageP elfI, Elf dest)
{
  elfGoTo(dest, 0);

  for (size_t i = 0; i < EI_NIDENT; i++)
  {
    elfWriteUC(dest, elfI->hdr.e_ident[i]);
  }

  // to update after changing type of elf (REL to EXEC)
  elfWrite16(dest, elfI->hdr.e_type);

  elfWrite16(dest, elfI->hdr.e_machine);
  elfWrite32(dest, elfI->hdr.e_version);
  elfWrite32(dest, elfI->hdr.e_entry);
  elfWrite32(dest, elfI->hdr.e_phoff);

  // to update after moving sh
  elfWrite32(dest, elfI->hdr.e_shoff);

  elfWrite32(dest, elfI->hdr.e_flags);
  elfWrite16(dest, elfI->hdr.e_ehsize);
  elfWrite16(dest, elfI->hdr.e_phentsize);
  elfWrite16(dest, elfI->hdr.e_phnum);
  elfWrite16(dest, elfI->hdr.e_shentsize);

  // to update after deleting/adding section(s)
  elfWrite16(dest, elfI->hdr.e_shnum);

  // to update after deleting/adding section(s)
  elfWrite16(dest, elfI->hdr.e_shstrndx);
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