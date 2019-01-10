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

void showHeader(ElfImageConstP elfI)
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

void showSectionHeader(ElfImageConstP elfI)
{
  printf("There are %d section headers, starting at offset 0x%x:\n\n", elfI->hdr.e_shnum,
         elfI->hdr.e_shoff);
  printf("Section Headers:\n");
  printf("  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf "
         "Al\n");
  for (int i = 0; i < elfI->hdr.e_shnum; i++)
  {
    char id[10];
    snprintf(id, 10, "  [%2d] ", i);
    fixPrint(id, 4);

    char const* str = getSectionString(elfI, i);
    fixPrint(str, 18);

    char const* strType = getElfType(sht, elfI->sections.tab[i].sh_type);
    fixPrint(strType, 16);

    printf("%.8x %.6x %.6x %.2x ", elfI->sections.tab[i].sh_addr,
           elfI->sections.tab[i].sh_offset, elfI->sections.tab[i].sh_size,
           elfI->sections.tab[i].sh_entsize);

    int x = 0;

    char flags[10];
    if (elfI->sections.tab[i].sh_flags & SHF_WRITE)
    {
      flags[x] = 'W';
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_ALLOC)
    {
      flags[x] = 'A';
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_EXECINSTR)
    {
      flags[x] = 'X';
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_MERGE)
    {
      flags[x] = 'M';
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_STRINGS)
    {
      flags[x] = 'S';
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_INFO_LINK)
    {
      flags[x] = 'I';
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_LINK_ORDER)
    {
      flags[x] = 'L';
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_GROUP)
    {
      flags[x] = 'G';
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_TLS)
    {
      flags[x] = 'T';
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_MASKOS)
    {
      flags[x] = 'o';
      x++;
    }
    if (elfI->sections.tab[i].sh_flags & SHF_MASKPROC)
    {
      flags[x] = 'p';
      x++;
    }
    flags[x] = '\0';
    fixPrintR(flags, 3);

    printf(" %2d", elfI->sections.tab[i].sh_link);

    printf(" %3d", elfI->sections.tab[i].sh_info);

    printf(" %2d \n", elfI->sections.tab[i].sh_addralign);
  }
  printf("\nKey to Flags:\n");
  printf("  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),\n"
         "  L (link order), O (extra OS processing required), G (group), T (TLS),\n"
         "  C (compressed), x (unknown), o (OS specific), E (exclude),\n"
         "  y (purecode), p (processor specific)");
}

void showSection(ElfImageConstP elfI, Elf32_Word sectionNo, unsigned char const* section)
{
  if (elfI->sections.tab[sectionNo].sh_size == 0)
  {
    printf("Section '%s' has no data to dump.\n", getSectionString(elfI, sectionNo));
  }
  else
  {
    Elf32_Word addr = elfI->sections.tab[sectionNo].sh_addr;

    printf("Hex dump of section '%s':\n", getSectionString(elfI, sectionNo));
    Elf32_Word size = elfI->sections.tab[sectionNo].sh_size;
    for (Elf32_Word i = 0; i < size;)
    {
      printf("  0x%.8x ", addr);
      size_t j;
      for (j = 0; j < 16 && i + j < size; j++)
      {
        printf("%.2x", section[i + j]);

        if (j % 4 == 3)
        {
          putchar(' ');
        }
      }

      // complete with spaces
      for (int spaces = j; spaces < 16; spaces++)
      {
        printf("  ");
        if (spaces % 4 == 3)
        {
          putchar(' ');
        }
      }

      printBytes(&section[i], j);
      putchar('\n');

      i += j;
      addr += 0x10;
    }
  }
}

void showSymbols(ElfImageConstP elfI)
{
  printf("Symbol table '.symtab' contains %i entries:\n", elfI->symbols.size);
  printf("   Num:    Value  Size Type    Bind   Vis      Ndx Name\n");

  for (size_t i = 0; i < elfI->symbols.size; i++)
  {
    // num
    char strNum[7];
    snprintf(strNum, 6, "%ld:", i);
    fixPrintR(strNum, 7);
    // value
    printf(" %.8x ", elfI->symbols.tab[i].st_value);
    // length
    snprintf(strNum, 5, "%d", elfI->symbols.tab[i].st_size);
    fixPrintR(strNum, 5);
    // type
    unsigned char info = elfI->symbols.tab[i].st_info;
    printf("%s", getElfType(stt, ELF32_ST_TYPE(info)));
    //link
    printf("%s", getElfType(stb, ELF32_ST_BIND(info)));
    // range
    printf("%s", getElfType(stv, ELF32_ST_VISIBILITY(elfI->symbols.tab[i].st_other)));
    // section index
    switch (elfI->symbols.tab[i].st_shndx)
    {
    case SHN_UNDEF:
      printf("UND ");
      break;
    case SHN_LOPROC:
      printf("LOP ");
      break;
    case SHN_HIPROC:
      printf("HIP ");
      break;
    case SHN_ABS:
      printf("ABS ");
      break;
    case SHN_COMMON:
      printf("COM ");
      break;
    case SHN_HIRESERVE:
      printf("HIR ");
      break;

    default:
      printf("%3i ", elfI->symbols.tab[i].st_shndx);
      break;
    }

    // name
    char const* strName = getSymbolString(elfI, i);

    fixPrint(strName, 20);
    putchar('\n');
  }
}
void showRelocations(ElfImageConstP elfI)
{
  if (elfI->rels.size == 0)
  {
    printf("There are no relocations in this file.\n");
  }

  for (size_t i = 0; i < elfI->rels.size; i++)
  {
    printf("Relocation section '%s' at offset 0x%x contains %i entries:\n",
           getSectionString(elfI, elfI->rels.tab[i].relSectionIdx),
           elfI->sections.tab[elfI->rels.tab[i].relSectionIdx].sh_offset,
           elfI->rels.tab[i].nbRel);

    if (elfI->rels.tab[i].relType == SHT_RELA)
    {
      printf(" Offset     Info    Type            Sym.Value  Sym. Name + Addend\n");
    }
    else
    {
      printf(" Offset     Info    Type            Sym.Value  Sym. Name\n");
    }

    for (size_t j = 0; j < elfI->rels.tab[i].nbRel; j++)
    {
      //offset
      printf("%.8x  ", elfI->rels.tab[i].rela[j].r_offset);
      //info
      printf("%.8x ", elfI->rels.tab[i].rela[j].r_info);
      //type
      Elf32_Word info = elfI->rels.tab[i].rela[j].r_info;
      fixPrint(getElfType(r_code, ELF32_R_TYPE(info)), 17);
      //valSym
      int symIdx = ELF32_R_SYM(info);
      printf(" %.8x", elfI->symbols.tab[symIdx].st_value);
      //name
      printf("   %s", getSymbolName(elfI, symIdx));
      // addend handle
      if (elfI->rels.tab[i].relType == SHT_RELA)
      {
        printf(" + %.8x", elfI->rels.tab[i].rela[j].r_addend);
      }
      printf("\n");
    }
  }
}
