#pragma once
#include "Elf.h"
#include "ElfReader.h"

/**
 * @brief parse the header of the elf file e
 * if e is not of class ELF32 the parsing stops before arch specific fields
 * e_ident is valid
 * @param elfI allocated ElfImage
 * @param e
 */
void parseHeader(ElfImageP elfI, Elf e);

/**
 * Parse the section header of the elf file e
 * @param elfI elfImage with header part initialized (parseHeader)
 * @param e Elf read/writer wrapper
 */
void parseSectionHeaders(ElfImageP elfI, Elf e);

unsigned char* getSectionWithName(Elf e,Elf32_Ehdr* hdr,char* name,int* size);

unsigned char* getSectionWithId(Elf e,Elf32_Ehdr* hdr,char* id,uint16_t* inf);

unsigned char* getSection(Elf e,uint16_t soff,uint16_t ssize);

/**
 * @brief parse the string table
 *
 * @param elfI elf image with header and section headers already parsed (parseHeader, parseSectionHeaders)
 * @param e Elf read/writer wrapper
 */
void parseStringTable(ElfImageP elfI, Elf e);
