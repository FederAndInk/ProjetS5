#include "ElfParser.h"
#include "ElfReader.h"
#include <linux/elf.h>
#include <stdio.h>

void unknown(uint32_t i)
{
  printf("<unknown: %d>\n", i);
}

void unknownX(uint32_t i)
{
  printf("<unknown: %x>\n", i);
}

void showHeader(Elf32_Ehdr hdr)
{
  printf("ELF Header:\n");
  printf("  Magic:   ");

  printf("%x", hdr.e_ident[0]);
  for (size_t i = 1; i < EI_NIDENT; i++)
  {
    printf(" %.2x", hdr.e_ident[i]);
  }
  printf("\n");
  printf("  Class:                             ");
  switch (hdr.e_ident[EI_CLASS])
  {
  case ELFCLASSNONE:
    printf("none\n");
    break;
  case ELFCLASS32:
    printf("ELF32\n");
    break;
  case ELFCLASS64:
    printf("ELF64\n");
    break;
  default:
    unknown(hdr.e_ident[EI_CLASS]);
    break;
  }
  printf("  Data:                              ");
  switch (hdr.e_ident[EI_DATA])
  {
  case ELFDATANONE:
    printf("none\n");
    break;
  case ELFDATA2LSB:
    printf("2's complement, little endian\n");
    break;
  case ELFDATA2MSB:
    printf("2's complement, big endian\n");
    break;
  default:
    unknown(hdr.e_ident[EI_DATA]);
    break;
  }
  printf("  Version:                           ");
  printf("%d", hdr.e_ident[EI_VERSION]);
  switch (hdr.e_ident[EI_VERSION])
  {
  case EV_NONE:
    printf("\n");
    break;
  case EV_CURRENT:
    printf(" (current)\n");
    break;
  default:
    printf(" <unknown: %%lx>\n");
    break;
  }
  printf("  OS/ABI:                            ");
  switch (hdr.e_ident[EI_OSABI])
  {
  case 0:
    printf("UNIX - System V\n");
    break;
  default:
    unknown(hdr.e_ident[EI_OSABI]);
    break;
  }
  printf("  ABI Version:                       %d\n", hdr.e_ident[EI_OSABI + 1]);
  printf("  Type:                              ");
  switch (hdr.e_type)
  {
  case ET_NONE:
    printf("none");
    break;
  case ET_REL:
    printf("REL (Relocatable file)\n");
    break;
  case ET_EXEC:
    printf("EXEC (Executable file)\n");
    break;
  case ET_DYN:
    printf("DYN (Shared object file)\n");
    break;
  case ET_CORE:
    printf("CORE (Core file)\n");
    break;
  default:
    unknown(hdr.e_type);
    break;
  }
  printf("  Machine:                           ");

  switch (hdr.e_machine)
  {
  case EM_NONE:
    printf("none\n");
    break;
  case EM_ARM:
    printf("ARM\n");
    break;
  case EM_X86_64:
    printf("X86_64\n");
    break;
  case EM_AARCH64:
    printf("AARCH64\n");
    break;
  case EM_386:
    printf("x86\n");
    break;
  default:
    unknownX(hdr.e_machine);
    break;
  }
  printf("  Version:                           0x%x\n", hdr.e_version);
  printf("  Entry point address:               0x%x\n", hdr.e_entry);
  printf("  Start of program headers:          %d (bytes into file)\n", hdr.e_phoff);
  printf("  Start of section headers:          %d (bytes into file)\n", hdr.e_shoff);
  printf("  Flags:                             0x%x\n", hdr.e_flags);
  printf("  Size of this header:               %d (bytes)\n", hdr.e_ehsize);
  printf("  Size of program headers:           %d (bytes)\n", hdr.e_phentsize);
  printf("  Number of program headers:         %d\n", hdr.e_phnum);
  printf("  Size of section headers:           %d (bytes)\n", hdr.e_shentsize);
  printf("  Number of section headers:         %d\n", hdr.e_shnum);
  printf("  Section header string table index: %d\n", hdr.e_shstrndx);
}

int main(int argc, char* argv[])
{
  // TODO: parse header and show it

  if (argc != 2)
  {
    printf("Usage: %s ELF_File\n", argv[0]);
    return 1;
  }

  Elf e = elfOpen(argv[1]);

  if (!isElf(e))
  {
    fprintf(stderr,
            "%s: Error: Not an ELF file - it has the wrong magic bytes at the start\n",
            argv[0]);
    return 1;
  }
  else
  {
    Elf32_Ehdr hdr;
    parseHeader(e, &hdr);
    if (hdr.e_ident[EI_CLASS] == ELFCLASS32)
    {
      showHeader(hdr);
      return 0;
    }
    else
    {
      fprintf(stderr, "Unsupported class, parses only ELF32\n");
      return 1;
    }
  }
}