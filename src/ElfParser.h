#pragma once
#include "ElfIO.h"
#include "ElfImage.h"
#include <stdbool.h>

/**
 * @brief parse the header of the elf file e
 * if e is not of class ELF32 the parsing stops before arch specific fields
 * e_ident is valid
 * @param elfI allocated ElfImage
 * @param e
 * @return true if the header has been parsed correctly and entirely
 * @return false otherwise (not ELF32)
 */
bool parseHeader(ElfImageP elfI, ElfFile e);

/**
 * Parse the section header of the elf file e
 * @param elfI elfImage with header part initialized (parseHeader)
 * @param e Elf reader/writer wrapper
 */
void parseSectionHeaders(ElfImageP elfI, ElfFile e);

/**
 * @brief parse the string table
 * 
 * @param elfI elf image with header and section headers already parsed (parseHeader, parseSectionHeaders)
 * @param e Elf reader/writer wrapper
 */
void parseStringTable(ElfImageP elfI, ElfFile e);

/**
 * @brief parse the symbol table
 * 
 * @param elfI elf image with header and section headers already parsed (parseHeader, parseSectionHeaders)
 * @param e Elf reader/writer wrapper
 */
void parseSymbolTable(ElfImageP elfI, ElfFile e);

/**
 * @brief parse the relocations table
 * 
 * @param elfI elf image with header and section headers already parsed (parseHeader, parseSectionHeaders)
 * @param e Elf reader/writer wrapper
 */
void parseRelocations(ElfImageP elfI, ElfFile e);

/**
 * @brief read an entire section as a chunk of bytes
 * 
 * @param elfI the pointer to the elf image
 * @param e the elf file handler
 * @param sectionNo the section id/no to read
 * @return unsigned char* bytes of the section dynamically allocated
 * @note the return value MUST BE FREE by 
 * @code
 * void free(void*)
 * @endcode
 */
unsigned char* readSection(ElfImageConstP elfI, ElfFile e, Elf32_Word sectionNo);

/**
 * @brief read a section as code (4 bytes instructions)
 * the section size must be a multiple of 4
 * 
 * @param elfI 
 * @param e 
 * @param sectionNo 
 * @param wantedEnd 
 * @return Elf32_Word* 
 */
Elf32_Word* readCode(ElfImageConstP elfI, ElfFile e, Elf32_Word sectionNo,
                     Endianness wantedEnd);

/**
 * @brief parse the entire elf
 * call:
 * - parseHeader
 * then if it is ELF32:
 * - parseSectionHeaders
 * - parseStringTable
 * - parseSymbolTable
 * - parseRelocations
 * 
 * @param elfI 
 * @param e 
 * @return true if the header has been parsed correctly and entirely
 * @return false otherwise (not ELF32)
 */
bool parseElf(ElfImageP elfI, ElfFile e);
