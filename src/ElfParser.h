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

/**
 * @brief parse the string table
 * 
 * @param elfI elf image with header and section headers already parsed (parseHeader, parseSectionHeaders)
 * @param e Elf read/writer wrapper
 */
void parseStringTable(ElfImageP elfI, Elf e);

/**
 * @brief parse the symbole table
 * 
 * @param elfI elf image with header and section headers already parsed (parseHeader, parseSectionHeaders)
 * @param e Elf read/writer wrapper
 */
void parseSymboleTable(ElfImageP elfI, Elf e);