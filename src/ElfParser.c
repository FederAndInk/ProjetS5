#include "ElfParser.h"
#include "ElfReader.h"
#include "ElfStringTable.h"
#include <stdlib.h>

void parseHeader(ElfImageP elfI, Elf e)
{
  elfGoTo(e, 0);

  for (size_t i = 0; i < EI_NIDENT; i++)
  {
    elfI->hdr.e_ident[i] = elfReadUC(e);
  }

  elfI->hdr.e_type = elfRead16(e);
  elfI->hdr.e_machine = elfRead16(e);
  elfI->hdr.e_version = elfRead32(e);

  if (elfI->hdr.e_ident[EI_CLASS] == ELFCLASS32)
  {
    elfI->hdr.e_entry = elfRead32(e);
    elfI->hdr.e_phoff = elfRead32(e);
    elfI->hdr.e_shoff = elfRead32(e);
    elfI->hdr.e_flags = elfRead32(e);
    elfI->hdr.e_ehsize = elfRead16(e);
    elfI->hdr.e_phentsize = elfRead16(e);
    elfI->hdr.e_phnum = elfRead16(e);
    elfI->hdr.e_shentsize = elfRead16(e);
    elfI->hdr.e_shnum = elfRead16(e);
    elfI->hdr.e_shstrndx = elfRead16(e);
  }
}

void parseSectionHeaders(ElfImageP elfI, Elf e)
{
  elfI->sections.size = elfI->hdr.e_shnum;

  elfI->sections.tab = (Elf32_Shdr*)malloc(elfI->sections.size * sizeof(Elf32_Shdr));
  elfGoTo(e, elfI->hdr.e_shoff);
  for (Elf32_Word i = 0; i < elfI->sections.size; i++)
  {
    elfI->sections.tab[i].sh_name = elfRead32(e);
    elfI->sections.tab[i].sh_type = elfRead32(e);
    elfI->sections.tab[i].sh_flags = elfRead32(e);
    elfI->sections.tab[i].sh_addr = elfRead32(e);
    elfI->sections.tab[i].sh_offset = elfRead32(e);
    elfI->sections.tab[i].sh_size = elfRead32(e);
    elfI->sections.tab[i].sh_link = elfRead32(e);
    elfI->sections.tab[i].sh_info = elfRead32(e);
    elfI->sections.tab[i].sh_addralign = elfRead32(e);
    elfI->sections.tab[i].sh_entsize = elfRead32(e);
  }
}

void parseStringTable(ElfImageP elfI, Elf e)
{
  elfI->strTable.secStrs =
      elfReadUC_s(e, elfI->sections.tab[elfI->hdr.e_shstrndx].sh_offset,
                  elfI->sections.tab[elfI->hdr.e_shstrndx].sh_size);

  Elf32_Word strIdx = getSectionIdFromStr(elfI, ".strtab");

  if (strIdx < elfI->sections.size)
  {
    elfI->strTable.symStrs = elfReadUC_s(e, elfI->sections.tab[strIdx].sh_offset,
                                         elfI->sections.tab[strIdx].sh_size);
  }
  else
  {
    fputs(".strtab not found", stderr);
  }
}

void parseSymboleTable(ElfImageP elfI, Elf e)
{

  Elf32_Word index = getSectionIdFromStr(elfI, ".symtab");
  elfI->symbols.size =
      elfI->sections.tab[index].sh_size / elfI->sections.tab[index].sh_entsize;
  elfI->symbols.tab = (Elf32_Sym*)malloc(elfI->symbols.size * sizeof(Elf32_Sym));
  Elf32_Off currentOffset = elfI->sections.tab[index].sh_offset;
  elfGoTo(e, currentOffset);
  for (size_t i = 0; i < (elfI->symbols.size); i++)
  {
    elfI->symbols.tab[i].st_name = elfRead32(e);
    elfI->symbols.tab[i].st_value = elfRead32(e);
    elfI->symbols.tab[i].st_size = elfRead32(e);
    elfI->symbols.tab[i].st_info = elfReadUC(e);
    elfI->symbols.tab[i].st_other = elfReadUC(e);
    elfI->symbols.tab[i].st_shndx = elfRead16(e);
  }
}
