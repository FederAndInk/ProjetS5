#pragma once
#include "ElfReader.h"
#include <elf.h>

/**
 * @brief parse the header of the elf file e
 * if e is not of class ELF32 the parsing stops before arch specific fields
 * e_ident is valid
 * @param e
 * @param hdr
 */
void parseHeader(Elf e, Elf32_Ehdr* hdr);




/*
    Parse the section header of the elf file e

    e must be a 32 bits elf file.
    shdr must be a valid pointeur

*/
void parseSectionHeader(Elf e, Elf32_Shdr tabshdr[],uint16_t shoff,uint16_t shnum);

unsigned char * getSectionWithName(Elf e,Elf32_Ehdr* hdr,char* name,int* size);

unsigned char * getSectionWithId(Elf e,Elf32_Ehdr* hdr,char* id,uint16_t* inf);

unsigned char * getSection(Elf e,uint16_t soff,uint16_t ssize);
