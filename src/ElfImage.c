#include "ElfImage.h"
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

  if (elfI->rels.tab)
  {
    for (size_t i = 0; i < elfI->rels.size; i++)
    {

      if (elfI->rels.tab[i].rela)
      {
        free(elfI->rels.tab[i].rela);
      }
    }

    free(elfI->rels.tab);
  }

  // TODO: add other free function for ElfImage
}
