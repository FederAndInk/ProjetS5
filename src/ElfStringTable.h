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
char const* getSectionString(ElfImageConstP elfI, Elf32_Word sectionNo);

/**
 * @brief Get the Symbol String name of the given symbol index into the symbol table
 * @note if the symbol is a section, no string is linked with the symbol directly
 * @see getSymbolName to also fetch the section name
 * 
 * @param elfI 
 * @param symbolIdx index of the symbol into the symbol table
 * @return char* 
 */
char const* getSymbolString(ElfImageConstP elfI, Elf32_Word symbolIdx);

/**
 * @brief Get the Symbol Name of the given symbol index into the symbol table
 * if the symbol is a section, it return the section name
 * 
 * @param elfI 
 * @param symbolIdx index of the symbol into the symbol table 
 * @return char* 
 */
char const* getSymbolName(ElfImageConstP elfI, Elf32_Word symbolIdx);

/**
 * @brief Get the Section Id From String
 * String can be the name of the section or a number as a string
 * 
 * @param elfI 
 * @param name 
 * @return Elf32_Word the section number or elfI->sections.size (sectionHdrs size) if no section found (out of range or no section with name str)
 */
Elf32_Word getSectionIdFromStr(ElfImageConstP elfI, char const* str);

/**
 * @brief Get the Symbol Id From String
 * String can be the name of the symbol or a number as a string
 * 
 * @param elfI 
 * @param str 
 * @return Elf32_Word 
 */
Elf32_Word getSymbolIdFromStr(ElfImageConstP elfI, char const* str);
