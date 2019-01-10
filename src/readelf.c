#include "ElfIO.h"
#include "ElfParser.h"
#include "ElfPrinter.h"
#include "ElfStringTable.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

void printHelp(char const* command)
{
  printf("Usage: %s args... elfFile\n", command);
  printf("Arguments:\n");
  printf("-h              show the header of elfFile\n");
  printf("-S              show section headers of elfFile\n");
  printf("-x sectionName  dump the section content of elfFile\n");
  printf("-s              show symbols of elfFile\n");
  printf("-r              show relocations of elfFile\n");
  printf("-H              show this help message\n");
}

#define HEADER_OPT 1
#define SECTION_HEADERS_OPT 2
#define HEX_DUMP_OPT 4
#define SYMBOLS_OPT 8
#define RELOCS_OPT 16

int main(int argc, char* argv[])
{

  if (argc < 3)
  {
    printHelp(argv[0]);
    return 1;
  }

  const char* sectionToDump = NULL;
  uint32_t    choosenOpts = 0;

  struct option opt[] = {{"header", no_argument, NULL, 'h'},
                         {"section-headers", no_argument, NULL, 'S'},
                         {"hex-dump", required_argument, NULL, 'x'},
                         {"symbols", required_argument, NULL, 's'},
                         {"relocs", required_argument, NULL, 'r'},
                         {"help", no_argument, NULL, 'H'}};

  int currOpt;
  while ((currOpt = getopt_long(argc, argv, "hSx:srH", opt, NULL)) != -1)
  {
    switch (currOpt)
    {
    case 'h':
      choosenOpts |= HEADER_OPT;
      break;
    case 'S':
      choosenOpts |= SECTION_HEADERS_OPT;
      break;
    case 'x':
      choosenOpts |= HEX_DUMP_OPT;
      sectionToDump = optarg;
      break;
    case 's':
      choosenOpts |= SYMBOLS_OPT;
      break;
    case 'r':
      choosenOpts |= RELOCS_OPT;
      break;
    case 'H':
      printHelp(argv[0]);
      return 0;
      break;
    default:
      printHelp(argv[0]);
      return 1;
      break;
    }
  }

  if (optind < argc)
  {
    ElfFile e = elfOpen(argv[optind]);

    if (!isElf(e))
    {
      fprintf(stderr,
              "%s: Error: Not an ELF file - it has the wrong magic bytes at the start\n",
              argv[0]);
      return 1;
    }
    else
    {
      ElfImage  elfI;
      ElfImageP elfIp = &elfI;
      initElfImage(elfIp);

      if (parseElf(elfIp, e))
      {

        if (choosenOpts & HEADER_OPT)
        {
          showHeader(elfIp);
          putchar('\n');
        }
        if (choosenOpts & SECTION_HEADERS_OPT)
        {
          showSectionHeader(elfIp);
          putchar('\n');
        }
        if (choosenOpts & HEX_DUMP_OPT)
        {
          Elf32_Word sectionId = getSectionIdFromStr(elfIp, sectionToDump);

          if (sectionId >= elfIp->sections.size)
          {
            printf("readelf: Warning: Section '%s' was not dumped because it does not "
                   "exist!\n",
                   sectionToDump);
          }
          else
          {
            unsigned char* sectionContent = readSection(elfIp, e, sectionId);
            showSection(elfIp, sectionId, sectionContent);
            free(sectionContent);
            putchar('\n');
          }
        }
        if (choosenOpts & SYMBOLS_OPT)
        {
          showSymbols(elfIp);
          putchar('\n');
        }
        if (choosenOpts & RELOCS_OPT)
        {
          showRelocations(elfIp);
          putchar('\n');
        }

        deleteElfImage(elfIp);
        elfClose(e);
        return 0;
      }
      else
      {
        fprintf(stderr, "Unsupported class, parses only ELF32\n");
        deleteElfImage(elfIp);
        elfClose(e);
        return 1;
      }
    }
  }
  else
  {
    printf("No Elf file provided\n");
    printHelp(argv[0]);
    return 1;
  }
}