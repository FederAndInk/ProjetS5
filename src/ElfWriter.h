#include "ElfIO.h"
#include "ElfImage.h"

void deleteRelocationSections(ElfImageP elfI);

void writeSections(ElfImageP elfI, ElfFile dest, ElfFile src);

void writeSymbols(ElfImageP elfI, ElfFile dest);

void writeSectionHeaders(ElfImageP elfI, ElfFile dest);

void setNewOffsets(ElfImageConstP elfI, ElfFile dest);
