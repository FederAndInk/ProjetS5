#include "ElfIO.h"
#include "ElfImage.h"

void deleteRelocationSections(ElfImageP elfI);

void writeSections(ElfImageP elfI, ElfFile dest, ElfFile src);

void writeSectionHeaders(ElfImageP elfI, ElfFile dest);

void setNewOffsets(ElfImageP elfI, ElfFile dest);
