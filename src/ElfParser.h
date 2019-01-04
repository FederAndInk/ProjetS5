#pragma once
#include "ElfImage.h"
#include "ElfReader.h"
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
bool parseHeader(ElfImageP elfI, Elf e);

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
unsigned char* readSection(ElfImageP elfI, Elf e, Elf32_Word sectionNo);

/**
 * @brief parse the entire elf
 * call:
 * - parseHeader
 * then if it is ELF32:
 * - parseSectionHeaders
 * - parseStringTable
 * - parseSymbols
 * - parseRelocations
 * 
 * @param elfI 
 * @param e 
 * @return true if the header has been parsed correctly and entirely
 * @return false otherwise (not ELF32)
 */
bool parseElf(ElfImageP elfI, Elf e);
