#pragma once
#include "ElfImage.h"

void showHeader(ElfImageP elfI);
void showSectionHeader(ElfImageP elfI);
void showSection(ElfImageP elfI, Elf32_Word sectionNo, unsigned char const* section);
void showSymbols(ElfImageP elfI);
void showRelocations(ElfImageP elfI);
