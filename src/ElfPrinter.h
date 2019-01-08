#pragma once
#include "ElfImage.h"

void showHeader(ElfImageConstP elfI);
void showSectionHeader(ElfImageConstP elfI);
void showSection(ElfImageConstP elfI, Elf32_Word sectionNo, unsigned char const* section);
void showSymbols(ElfImageConstP elfI);
void showRelocations(ElfImageConstP elfI);
