#include "ElfString.h"
#include <elf.h>
#include <stdlib.h>

char const ELF_UNKNOWN[] = "Unknown";

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
                 {-1, ELF_UNKNOWN}};

ElfType stb[] = {{STB_LOCAL, "LOCAL  "},  {STB_GLOBAL, "GLOBAL "}, {STB_HIOS, "HIOS   "},
                 {STB_LOPROC, "LOPROS "}, {STB_WEAK, "WEAK   "},   {STB_LOOS, "LOOS   "},
                 {STB_HIPROC, "HIPROC "}, {-1, ELF_UNKNOWN}};

ElfType stt[] = {
    {STT_NOTYPE, " NOTYPE  "},    {STT_OBJECT, " OBJECT  "}, {STT_FUNC, " FUNCTION"},
    {STT_SECTION, " SECTION "},   {STT_FILE, " FILE    "},   {STT_COMMON, " COMMON   "},
    {STT_TLS, " TLS     "},       {STT_NUM, " NUM     "},    {STT_LOOS, " LOOS    "},
    {STT_GNU_IFUNC, " GNUIFCT "}, {STT_HIOS, " HIOS     "},  {STT_LOPROC, " LOPROC  "},
    {STT_HIPROC, " HIPROC   "},   {-1, ELF_UNKNOWN}};

ElfType stv[] = {{STV_DEFAULT, "DEFAULT  "},
                 {STV_INTERNAL, "INTERN   "},
                 {STV_HIDDEN, "HIDDEN   "},
                 {STV_PROTECTED, "PROTECT  "},
                 {-1, ELF_UNKNOWN}};

char const* getElfType(ElfType t[], long i)
{
  int ind = 0;
  while (t[ind].name != ELF_UNKNOWN && t[ind].i != i)
  {
    ind++;
  }

  return t[ind].name;
}
