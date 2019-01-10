#include "ElfWriter.h"
#include "ElfIO.h"
#include "ElfParser.h"
#include "ElfString.h"
#include "ElfStringTable.h"
#include "util.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
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

  for (size_t k = 0; k < elfI->rels.size; k++)
  {
    if (elfI->rels.tab[k].sectionIdx > minNdx)
    {
      elfI->rels.tab[k].sectionIdx--;
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

void copySections(ElfImageP elfI, ElfFile dest, ElfFile src)
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

void writeSymbols(ElfImageP elfI, ElfFile dest)
{
  long offSave = elfTell(dest);
  elfGoTo(dest, elfI->sections.tab[getSectionIdFromStr(elfI, ".symtab")].sh_offset);
  for (Elf32_Word i = 0; i < elfI->symbols.size; i++)
  {
    elfWrite32(dest, elfI->symbols.tab[i].st_name);
    elfWrite32(dest, elfI->symbols.tab[i].st_value);
    elfWrite32(dest, elfI->symbols.tab[i].st_size);
    elfWriteUC(dest, elfI->symbols.tab[i].st_info);
    elfWriteUC(dest, elfI->symbols.tab[i].st_other);
    elfWrite16(dest, elfI->symbols.tab[i].st_shndx);
  }
  elfGoTo(dest, offSave);
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

bool getBit(Elf32_Word nb, size_t no)
{
  assert(no < 32);
  return (nb >> no) & 1;
}

Elf32_Word truncate(Elf32_Word nbToTruncate, size_t max)
{
  assert(max < 32);

  Elf32_Word mask = 1;
  for (size_t i = 0; i < max; i++)
  {
    mask = (mask << 1) | 1;
  }
  nbToTruncate &= mask;

  return nbToTruncate;
}

int32_t signExtend(Elf32_Word nb, size_t bit)
{
  if (getBit(nb, bit) == 1)
  {
    Elf32_Word mask = 1;
    for (size_t i = 0; i < bit; i++)
    {
      mask = (mask << 1) | 1;
    }
    nb |= ~mask;
  }
  return nb;
}

void applyRelocation(ElfImageP elfI, ElfFile dest)
{
  for (size_t i = 0; i < elfI->rels.size; i++)
  {

    for (size_t k = 0; k < elfI->rels.tab[i].nbRel; k++)
    {
      Elf32_Word    info = elfI->rels.tab[i].rela[k].r_info;
      Elf32_Word    symIdx = ELF32_R_SYM(info);
      unsigned char relType = ELF32_R_TYPE(info);

      elfGoTo(dest, elfI->sections.tab[elfI->rels.tab[i].sectionIdx].sh_offset);
      elfGoToRel(dest, elfI->rels.tab[i].rela[k].r_offset);
      Elf32_Word inst = elfRead32(dest); // to transform in a
      int32_t    A;
      Elf32_Word S = elfI->symbols.tab[symIdx].st_value;
      Elf32_Word P = elfI->sections.tab[elfI->rels.tab[i].sectionIdx].sh_addr +
                     elfI->rels.tab[i].rela[k].r_offset;
      Elf32_Word X;
      Elf32_Word mask;
      switch (relType)
      {
      case R_ARM_ABS32:
        A = inst;
        X = S + A;
        mask = 0xFFFFFFFF;
        break;
      case R_ARM_ABS16:
        A = signExtend(truncate(inst, 16), 16);
        X = S + A;
        mask = 0xFFFF;
        break;
      case R_ARM_ABS8:
        A = signExtend(truncate(inst, 8), 8);
        X = S + A;
        mask = 0xFF;
        break;
      case R_ARM_CALL:
      case R_ARM_JUMP24:
        A = signExtend(truncate(inst, 23) << 2, 23);
        X = S + A - P;
        mask = 0x03FFFFFE;
        inst &= ~0x00FFFFFF;
        break;
      }
      X = (X & mask) >> 2;
      Elf32_Word newWord = inst | X;
      elfGoTo(dest, elfI->sections.tab[elfI->rels.tab[i].sectionIdx].sh_offset);
      elfGoToRel(dest, elfI->rels.tab[i].rela[k].r_offset);
      elfWrite32(dest, newWord);
    }
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