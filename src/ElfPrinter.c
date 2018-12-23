#include "ElfPrinter.h"
#include "ElfString.h"
#include "ElfStringTable.h"
#include "util.h"
#include <stdio.h>

void unknown(uint32_t i)
{
  printf("<unknown: %d>\n", i);
}

void unknownX(uint32_t i)
{
  printf("<unknown: %x>\n", i);
}

void showHeader(ElfImageP elfI)
{
  printf("ELF Header:\n");
  printf("  Magic:   ");

  printf("%x", elfI->hdr.e_ident[0]);
  for (size_t i = 1; i < EI_NIDENT; i++)
  {
    printf(" %.2x", elfI->hdr.e_ident[i]);
  }
  printf("\n");
  printf("  Class:                             ");
  switch (elfI->hdr.e_ident[EI_CLASS])
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
    unknown(elfI->hdr.e_ident[EI_CLASS]);
    break;
  }
  printf("  Data:                              ");
  switch (elfI->hdr.e_ident[EI_DATA])
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
    unknown(elfI->hdr.e_ident[EI_DATA]);
    break;
  }
  printf("  Version:                           ");
  printf("%d", elfI->hdr.e_ident[EI_VERSION]);
  switch (elfI->hdr.e_ident[EI_VERSION])
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
  switch (elfI->hdr.e_ident[EI_OSABI])
  {
  case ELFOSABI_SYSV:
    printf("UNIX - System V\n");
    break;
  default:
    unknown(elfI->hdr.e_ident[EI_OSABI]);
    break;
  }
  printf("  ABI Version:                       %d\n", elfI->hdr.e_ident[EI_OSABI + 1]);
  printf("  Type:                              ");
  switch (elfI->hdr.e_type)
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
    unknown(elfI->hdr.e_type);
    break;
  }
  printf("  Machine:                           ");

  switch (elfI->hdr.e_machine)
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
    unknownX(elfI->hdr.e_machine);
    break;
  }
  printf("  Version:                           0x%x\n", elfI->hdr.e_version);
  printf("  Entry point address:               0x%x\n", elfI->hdr.e_entry);
  printf("  Start of program headers:          %d (bytes into file)\n",
         elfI->hdr.e_phoff);
  printf("  Start of section headers:          %d (bytes into file)\n",
         elfI->hdr.e_shoff);
  printf("  Flags:                             0x%x\n", elfI->hdr.e_flags);
  printf("  Size of this header:               %d (bytes)\n", elfI->hdr.e_ehsize);
  printf("  Size of program headers:           %d (bytes)\n", elfI->hdr.e_phentsize);
  printf("  Number of program headers:         %d\n", elfI->hdr.e_phnum);
  printf("  Size of section headers:           %d (bytes)\n", elfI->hdr.e_shentsize);
  printf("  Number of section headers:         %d\n", elfI->hdr.e_shnum);
  printf("  Section header string table index: %d\n", elfI->hdr.e_shstrndx);
}

void showSectionHeader(ElfImageP elfI)
{
  printf("There are %d section headers, starting at offset 0x%x:\n", elfI->hdr.e_shnum,
         elfI->hdr.e_shoff);
  printf("Section Headers:\n");
  printf("Nr  Name                Type             Address  Offset Size   ES Flags Link  "
         "Info  Align\n");
  for (int i = 0; i < elfI->hdr.e_shnum; i++)
  {
    char id[4];
    sprintf(id, "%d", i);
    fixPrint(id, 4);

    char* str = getSectionString(elfI, i);
    fixPrint(str, 20); 

    char const* strType = getElfType(sht, elfI->sections.tab[i].sh_type);
    fixPrint(strType, 17);

    printf("%.8x %.6x %.6x %.2x ", elfI->sections.tab[i].sh_addr,
           elfI->sections.tab[i].sh_offset, elfI->sections.tab[i].sh_size,
           elfI->sections.tab[i].sh_entsize);

    int x = 0;

    if (elfI->sections.tab[i].sh_flags & SHF_WRITE)
    {
      printf("W");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_ALLOC)
    {
      printf("A");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_EXECINSTR)
    {
      printf("X");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_MERGE)
    {
      printf("M");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_STRINGS)
    {
      printf("S");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_INFO_LINK)
    {
      printf("I");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_LINK_ORDER)
    {
      printf("L");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_GROUP)
    {
      printf("G");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_TLS)
    {
      printf("T");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_MASKOS)
    {
      printf("o");
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_MASKPROC)
    {
      printf("p");
      x++;
    }

    for (int j = 0; j < (5 - x); j++)
      putchar(' ');

    char lnk[16];
    printf(" ");
    sprintf(lnk, "%d", elfI->sections.tab[i].sh_link);
    fixPrint(lnk, 5);

    char info[16];
    printf(" ");
    sprintf(info, "%d", elfI->sections.tab[i].sh_info);
    fixPrint(info, 6);

    printf("%d \n", elfI->sections.tab[i].sh_addralign);
  }
  printf("\nKey to Flags:\n");
  printf("W (write), A (alloc), X (execute), M (merge), S (strings), l (large)\n");
  printf("I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)\n");
  printf("O (extra OS processing required) o (OS specific), p (processor specific)\n");
}

void showSection(ElfImageP elfI, char const* sectionName)
{
  printf("WIP");
}
void showSymbols(ElfImageP elfI)
{
  printf("WIP");
}
void showRelocations(ElfImageP elfI)
{
  printf("WIP");
}
