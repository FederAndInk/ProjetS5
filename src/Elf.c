#include "Elf.h"
#include <stdlib.h>
#include <string.h>

void initElfImage(ElfImageP elfI)
{
  memset(elfI, 0, sizeof(ElfImage));
}

void deleteElfImage(ElfImageP elfI)
{

  if (elfI->sections.tab)
  {
    free(elfI->sections.tab);
  }

  if (elfI->strTable.secStrs)
  {
    free(elfI->strTable.secStrs);
  }

  if (elfI->strTable.symStrs)
  {
    free(elfI->strTable.symStrs);
  }

  if (elfI->symbols.tab)
  {
    free(elfI->symbols.tab);
  }

  // TODO: add other free function for ElfImage
}
