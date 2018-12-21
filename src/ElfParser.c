#include "ElfParser.h"
#include "ElfReader.h"
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ElfStringTable.h"

void parseHeader(ElfImageP elfI, Elf e)
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
  }
}

void parseSectionHeaders(ElfImageP elfI, Elf e)
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

void parseStringTable(ElfImageP elfI, Elf e)
{
  elfI->strTable.secStrs =
      elfReadUC_s(e, elfI->sections.tab[elfI->hdr.e_shstrndx].sh_offset,
                  elfI->sections.tab[elfI->hdr.e_shstrndx].sh_size);

  Elf32_Word strIdx = getSectionIdFromStr(elfI, ".strtab");

  if (strIdx < elfI->sections.size)
  {
    elfI->strTable.symStrs = elfReadUC_s(e, elfI->sections.tab[strIdx].sh_offset,
                                         elfI->sections.tab[strIdx].sh_size);
  }else {
    fputs(".strtab not found", stderr);
  }
}

unsigned char * getSectionWithName(Elf e,Elf32_Ehdr* hdr,char* name,int* size)
{
        Elf32_Shdr tabshdr[hdr->e_shnum];
        parseSectionHeader(e, tabshdr,hdr->e_shoff,hdr->e_shnum);
        int id;
        for (int i = 0 ; i<hdr->e_shnum;i++)
        {
            if (strcmp(StrtableGetString(e,(tabshdr[hdr->e_shstrndx].sh_offset+tabshdr[i].sh_name)),name))
            {
               id=i;
            }
        }

        return getSectionWithId(e, hdr, id,size);



}


unsigned char * getSectionWithId(Elf e,Elf32_Ehdr* hdr,char* id,uint16_t * inf)
{
    int i = atoi(id);

    if (i > 0 && i < hdr->e_shnum)
    {
        Elf32_Shdr tabshdr[hdr->e_shnum];
        parseSectionHeader(e, tabshdr,hdr->e_shoff,hdr->e_shnum);
        inf[0]=tabshdr[i].sh_size;
        inf[1]=tabshdr[i].sh_offset;
        return getSection(e,tabshdr[i].sh_offset,tabshdr[i].sh_size);
    }
    else
    {
        return NULL;
    }
}


unsigned char * getSection(Elf e,uint16_t soff,uint16_t ssize)
{
    elfGoTo(e,soff);
    unsigned char* str=malloc(sizeof(unsigned char)*ssize);
    for (int i = 0 ; i<ssize ; i++)
    {
        str[i]=elfReadUC(e);
    }
    return str;
}
