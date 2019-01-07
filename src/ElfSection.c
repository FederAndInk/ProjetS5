#include "Elf.h"
#include "ElfParser.h"
#include "ElfReader.h"
#include "ElfString.h"
#include "util.h"
#include "ElfStringTable.h"
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>

void showSection(uint16_t soff,uint16_t size,Elf e)
{

}

int main(int argc, char* argv[])
{

  if (argc != 3)
  {
    printf("Usage: %s ELF_File Numsection or %s ELF_File SectionName \n", argv[0]);
    return 1;
  }

  Elf e = elfOpen(argv[1]);

  if (!isElf(e))
  {
    fprintf(stderr,
            "%s: Error: Not an ELF file - it has the wrong magic bytes at the start\n",
            argv[0]);
    return 1;
  }
  else
  {
    //Elf32_Ehdr hdr;
    //parseHeader(e, &hdr);

    ElfImage  elfI;
    ElfImageP elfIp = &elfI;
    initElfImage(elfIp);

    parseHeader(elfIp, e);

    if (elfIp->hdr.e_ident[EI_CLASS] == ELFCLASS32)
    {
        int arr[] = {11, 15, 6, 8, 9, 10};

        arrayRemove(arr,2,6,2);
        for (int i=0;i<4;i++)
        {
            printf("%d",arr[i]);
        }

      return 0;
    }
    else
    {
      fprintf(stderr, "Unsupported class, parses only ELF32\n");
      return 1;
    }
  }
}
