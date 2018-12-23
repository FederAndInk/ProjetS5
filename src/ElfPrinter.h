#pragma once
#include "ElfImage.h"

void showHeader(ElfImageP elfI);
void showSectionHeader(ElfImageP elfI);
void showSection(ElfImageP elfI, char const* sectionName);
void showSymbols(ElfImageP elfI);
void showRelocations(ElfImageP elfI);
