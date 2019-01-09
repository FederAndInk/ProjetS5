#include "ElfIO.h"
#include "ElfImage.h"
#include "ElfParser.h"
#include "ElfStringTable.h"
#include "arm_simulator_interface.h"
#include "debug.h"
#include <elf.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

void addSection(arm_simulator_data_t simulator, ElfImageConstP elfI, char const* nom, char*)
{
  Elf32_Shdr currentSect = elfI->sections.tab[getSectionIdFromStr(elfI, nom)];

  unsigned char* code = readSection(elfI, , Elf32_Word sectionNo);
  
  if (elfIsBigEndian(elfI)) {
    
  }
  

  arm_write_memory(simulator, currentSect.sh_addr, code, currentSect.sh_entsize);
}