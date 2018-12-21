#include "ElfParser.h"
#include "ElfReader.h"
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void parseSectionHeader(Elf e, Elf32_Shdr tabshdr[],uint16_t shoff,uint16_t shnum)
{
    elfGoTo(e,shoff);
    for (int i = 0 ; i < shnum ; i++)
    {
        tabshdr[i].sh_name=elfRead32(e);
        tabshdr[i].sh_type=elfRead32(e);
        tabshdr[i].sh_flags=elfRead32(e);
        tabshdr[i].sh_addr=elfRead32(e);
        tabshdr[i].sh_offset=elfRead32(e);
        tabshdr[i].sh_size=elfRead32(e);
        tabshdr[i].sh_link=elfRead32(e);
        tabshdr[i].sh_info=elfRead32(e);
        tabshdr[i].sh_addralign=elfRead32(e);
        tabshdr[i].sh_entsize=elfRead32(e);
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
