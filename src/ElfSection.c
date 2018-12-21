#include "ElfParser.h"
#include "ElfReader.h"
#include "ElfString.h"
#include "util.h"
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
    Elf32_Ehdr hdr;
    parseHeader(e, &hdr);
    if (hdr.e_ident[EI_CLASS] == ELFCLASS32)
    {
        uint16_t inf[2];
        unsigned char* sec=getSectionWithId(e,&hdr,argv[2],&inf);
        for (int i =0;i<(inf[0]/4);i++)
        {
            printf("%.2x",sec[i]);
            printf("%.2x",sec[i+1]);
            printf("%.2x",sec[i+2]);
            printf("%.2x ",sec[i+3]);

            for (int j=i;j<i+4;j++)
            {
                if (sec[j]>32 && sec[j]<127)
                    printf("%c",sec[j]);
                else printf(".");
            }
            /*
            printf("%c",sec[i]);
            printf("%c",sec[i+1]);
            printf("%c",sec[i+2]);
            printf("%c ",sec[i+3]);
            */
            if (i%4==3)
                printf("\n");
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
