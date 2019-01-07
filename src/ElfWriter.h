#include "Elf.h"
#include "ElfReader.h"

ElfImageP deleteRelocationSections(ElfImageP elfI);

void writeSections(ElfImageP elfI, Elf dest, Elf src);

void writeSectionHeaders(ElfImageP elfI, Elf dest);

void setNewOffsets(ElfImageP elfI, Elf dest);
