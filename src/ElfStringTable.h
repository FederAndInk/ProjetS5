#pragma once
#include "ElfImage.h"
#include "ElfReader.h"

char* getSectionString(ElfImageP elfI, Elf32_Word sectionNo);

char* getSymbolString(ElfImageP elfI, Elf32_Word offIntoTab);

/**
 * @brief Get the Section Id From String
 * String can be the name of the section or a number as a string
 * 
 * @param elfI 
 * @param name 
 * @return Elf32_Word the section number or elfI->sections.size (sectionHdrs size) if no section found (out of range or no section with name str)
 */
Elf32_Word getSectionIdFromStr(ElfImageP elfI, char const* str);
