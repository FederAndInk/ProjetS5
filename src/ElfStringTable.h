#pragma once
#include "ElfIO.h"
#include "ElfImage.h"

/**
 * @brief Get the Section String name of the given section number
 * 
 * @param elfI 
 * @param sectionNo the section number
 * @return char* 
 */
char* getSectionString(ElfImageP elfI, Elf32_Word sectionNo);

/**
 * @brief Get the Symbol String name of the given symbol index into the symbol table
 * @note if the symbol is a section, no string is linked with the symbol directly
 * @see getSymbolName to also fetch the section name
 * 
 * @param elfI 
 * @param symbolIdx index of the symbol into the symbol table
 * @return char* 
 */
char* getSymbolString(ElfImageP elfI, Elf32_Word symbolIdx);

/**
 * @brief Get the Symbol Name of the given symbol index into the symbol table
 * if the symbol is a section, it return the section name
 * 
 * @param elfI 
 * @param symbolIdx index of the symbol into the symbol table 
 * @return char* 
 */
char* getSymbolName(ElfImageP elfI, Elf32_Word symbolIdx);

/**
 * @brief Get the Section Id From String
 * String can be the name of the section or a number as a string
 * 
 * @param elfI 
 * @param name 
 * @return Elf32_Word the section number or elfI->sections.size (sectionHdrs size) if no section found (out of range or no section with name str)
 */
Elf32_Word getSectionIdFromStr(ElfImageP elfI, char const* str);
