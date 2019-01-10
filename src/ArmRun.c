#include "ElfIO.h"
#include "ElfImage.h"
#include "ElfParser.h"
#include "ElfStringTable.h"
#include "arm_simulator_interface.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

void addSection(arm_simulator_data_t simulator, ElfImageConstP elfI, ElfFile e,
                Elf32_Word sectionNo)
{
  Elf32_Shdr currentSect = elfI->sections.tab[sectionNo];

  // SEE: if we want ENDIAN_LITTLE or other for arm_write_memory
  Elf32_Word* code = readCode(elfI, e, sectionNo, ENDIAN_LITTLE);

  arm_write_memory(simulator, currentSect.sh_addr, code, currentSect.sh_size);
}

int main(int argc, char* argv[]) {}
