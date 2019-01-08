#include "Elf.h"
#include "ElfReader.h"

void deleteRelocationSections(ElfImageP elfI);

void writeElfHeader(ElfImageP elfI, Elf dest);

void writeSections(ElfImageP elfI, Elf dest, Elf src);

void writeSectionHeaders(ElfImageP elfI, Elf dest);

void setNewOffsets(ElfImageP elfI, Elf dest);
