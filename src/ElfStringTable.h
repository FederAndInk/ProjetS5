#pragma once
#include "Elf.h"
#include "ElfReader.h"

char* getSectionString(ElfImageP elfI, Elf32_Word sectionNo);

char* getSymbolString(ElfImageP elfI, Elf32_Word offIntoTab);

Elf32_Word getSectionIdFromStr(ElfImageP elfI, char const* name);
