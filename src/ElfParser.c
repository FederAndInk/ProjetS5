#include "ElfParser.h"
#include "ElfIO.h"
#include "util.h"
#include "ElfStringTable.h"
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

bool parseHeader(ElfImageP elfI, ElfFile e)
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
    return true;
  }
  else
  {
    return false;
  }
}

void parseSectionHeaders(ElfImageP elfI, ElfFile e)
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

void parseStringTable(ElfImageP elfI, ElfFile e)
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

void parseSymbolTable(ElfImageP elfI, ElfFile e)
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

void parseRelocations(ElfImageP elfI, ElfFile e)
{
  size_t nbrel = 0;
  int    table[elfI->sections.size];
  for (size_t i = 0; i < elfI->sections.size; i++)
  {

    if (elfI->sections.tab[i].sh_type == SHT_REL ||
        elfI->sections.tab[i].sh_type == SHT_RELA)
    {
      table[nbrel] = i;
      nbrel++;
    }
  }
  elfI->rels.size = nbrel;
  elfI->rels.tab = (Elf32Rels*)malloc(nbrel * sizeof(Elf32Rels));
  for (size_t i = 0; i < nbrel; i++)
  {
    Elf32_Off currentOffset = elfI->sections.tab[table[i]].sh_offset;
    elfGoTo(e, currentOffset);
    elfI->rels.tab[i].relType = elfI->sections.tab[table[i]].sh_type;
    elfI->rels.tab[i].nbRel =
        elfI->sections.tab[table[i]].sh_size / elfI->sections.tab[table[i]].sh_entsize;
    elfI->rels.tab[i].rela = malloc(sizeof(Elf32_Rela) * elfI->rels.tab[i].nbRel);
    elfI->rels.tab[i].relSectionIdx = table[i];
    elfI->rels.tab[i].sectionIdx = elfI->sections.tab[table[i]].sh_info;
    for (size_t j = 0; j < elfI->rels.tab[i].nbRel; j++)
    {

      elfI->rels.tab[i].rela[j].r_offset = elfRead32(e);
      elfI->rels.tab[i].rela[j].r_info = elfRead32(e);

      if (elfI->rels.tab[i].relType == SHT_RELA)
      {
        elfI->rels.tab[i].rela[j].r_addend = elfRead32(e);
      }
    }
  }
}

unsigned char* readSection(ElfImageConstP elfI, ElfFile e, Elf32_Word sectionNo)
{
  Elf32_Shdr*    sH = &elfI->sections.tab[sectionNo];
  unsigned char* section = elfReadUC_s(e, sH->sh_offset, sH->sh_size);
  return section;
}

Elf32_Word* readCode(ElfImageConstP elfI, ElfFile e, Elf32_Word sectionNo,
                     Endianness wantedEnd)
{
  Elf32_Shdr* sH = &elfI->sections.tab[sectionNo];
  assert((sH->sh_size % 4) == 0);
  Elf32_Word* code = (Elf32_Word*)readSection(elfI, e, sectionNo);

  if (!elfIsEndianness(e, wantedEnd)) {
    Elf32_Word* codeEnd = code + sH->sh_size;
    for (Elf32_Word* codeTmp = code; codeTmp != codeEnd; codeTmp++)
    {
      reverseEndianess(codeTmp, sizeof(Elf32_Word));
    }
  }

  return code;
}

bool parseElf(ElfImageP elfI, ElfFile e)
{
  if (parseHeader(elfI, e))
  {
    parseSectionHeaders(elfI, e);
    parseStringTable(elfI, e);
    parseSymbolTable(elfI, e);
    parseRelocations(elfI, e);
    return true;
  }
  else
  {
    return false;
  }
}
