#pragma once
#include <elf.h>

typedef struct
{
  Elf32_Shdr* tab;
  Elf32_Word  size;
} Elf32Sections;

typedef struct
{
  unsigned char* secStrs;
  unsigned char* symStrs;
} Elf32StringTable;

typedef struct
{
  Elf32_Sym* tab;
  Elf32_Word size;
} Elf32Symbols;

typedef struct
{
  unsigned char* tab;
  Elf32_Word size;
  Elf32_Word address;
} Elf32Section;

typedef struct
{
  Elf32_Ehdr       hdr;
  Elf32Sections    sections;
  Elf32StringTable strTable;
  Elf32Symbols     symbols; // TODO: complete with other elf parts
} ElfImage;

typedef ElfImage* ElfImageP;

void initElfImage(ElfImageP elfI);

void deleteElfImage(ElfImageP elfI);
