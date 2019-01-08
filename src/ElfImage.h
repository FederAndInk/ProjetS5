#pragma once
#include <elf.h>

typedef struct
{
  Elf32_Shdr* tab;
  Elf32_Word  size; // nb elements
} Elf32Sections;

typedef struct
{
  unsigned char* secStrs;
  unsigned char* symStrs;
} Elf32StringTable;

typedef struct
{
  Elf32_Sym* tab;
  Elf32_Word size; // nb elements
} Elf32Symbols;

typedef struct
{
  Elf32_Rela* rela;
  Elf32_Word  relType; // SHT_RELA ou SHT_REL
  Elf32_Word  nbRel;
  // rel section id
  Elf32_Word sectionIdx;
} Elf32Rels;

typedef struct
{
  Elf32Rels* tab;
  Elf32_Word size; // nb elements
} Elf32RelsTab;

typedef struct
{
  Elf32_Ehdr       hdr;
  Elf32Sections    sections;
  Elf32StringTable strTable;
  Elf32Symbols     symbols;
  Elf32RelsTab     rels;
} ElfImage;

typedef ElfImage* ElfImageP;

typedef ElfImage const* ElfImageConstP;

void initElfImage(ElfImageP elfI);

void deleteElfImage(ElfImageP elfI);
