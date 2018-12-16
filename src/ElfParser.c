#include "ElfParser.h"
#include "ElfReader.h"

void parseHeader(Elf e, Elf32_Ehdr* hdr)
{
  elfGoTo(e, 0);

  for (size_t i = 0; i < EI_NIDENT; i++)
  {
    hdr->e_ident[i] = elfReadUC(e);
  }

  hdr->e_type = elfRead16(e);
  hdr->e_machine = elfRead16(e);
  hdr->e_version = elfRead32(e);

  if (hdr->e_ident[EI_CLASS] == ELFCLASS32)
  {
    hdr->e_entry = elfRead32(e);
    hdr->e_phoff = elfRead32(e);
    hdr->e_shoff = elfRead32(e);
    hdr->e_flags = elfRead32(e);
    hdr->e_ehsize = elfRead16(e);
    hdr->e_phentsize = elfRead16(e);
    hdr->e_phnum = elfRead16(e);
    hdr->e_shentsize = elfRead16(e);
    hdr->e_shnum = elfRead16(e);
    hdr->e_shstrndx = elfRead16(e);
  }
}
