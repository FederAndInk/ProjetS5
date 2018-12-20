#include "ElfString.h"
#include <elf.h>
#include <stdlib.h>

ElfType sht[] = {{SHT_NULL, "NULL"},
                 {SHT_PROGBITS, "PROGBITS"},
                 {SHT_SYMTAB, "SYMTAB"},
                 {SHT_STRTAB, "STRTAB"},
                 {SHT_RELA, "RELA"},
                 {SHT_HASH, "HASH"},
                 {SHT_DYNAMIC, "DYNAMIC"},
                 {SHT_NOTE, "NOTE"},
                 {SHT_NOBITS, "NOBITS"},
                 {SHT_REL, "REL"},
                 {SHT_SHLIB, "SHLIB"},
                 {SHT_DYNSYM, "DYNSYM"},
                 {SHT_INIT_ARRAY, "INIT_ARRAY"},
                 {SHT_FINI_ARRAY, "FINI_ARRAY"},
                 {SHT_PREINIT_ARRAY, "PREINIT_ARRAY"},
                 {SHT_GROUP, "GROUP"},
                 {SHT_SYMTAB_SHNDX, "SYMTAB_SHNDX"},
                 {SHT_LOOS, "LOOS"},
                 {SHT_HIOS, "HIOS"},
                 {SHT_LOPROC, "LOPROC"},
                 {SHT_HIPROC, "HIPROC"},
                 {SHT_LOUSER, "LOUSER"},
                 {SHT_HIUSER, "HIUSER"},
                 {SHT_ARM_EXIDX, "ARM_EXIDX"},
                 {SHT_ARM_ATTRIBUTES, "ARM_ATTRIBUTES"},
                 {SHT_ARM_PREEMPTMAP, "ARM_PREEMPTMAP"},
                 {-1, NULL}};

char const* getElfType(ElfType t[], long i)
{
  int ind = 0;
  while (t[ind].name != NULL && t[ind].i != i)
  {
    ind++;
  }

  return t[ind].name;
}
