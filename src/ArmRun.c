#include "ElfIO.h"
#include "ElfImage.h"
#include "ElfParser.h"
#include "ElfStringTable.h"
#include "arm_simulator_interface.h"
#include "debug.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
void usage(char* name)
{
  fprintf(stderr,
          "Usage:\n"
          "%s [ --help ] [ --host hostname ] [ --service servicename ] [ --debug file ] "
          "file\n\n"
          "Loads a sample ARM code to a remote simulator. The --debug flag enables the "
          "output produced by "
          "calls to the debug function in the named source file.\n",
          name);
}

void addSection(arm_simulator_data_t simulator, ElfImageConstP elfI, ElfFile e,
                Elf32_Word sectionNo)
{
  Elf32_Shdr currentSect = elfI->sections.tab[sectionNo];

  // SEE: if we want ENDIAN_LITTLE or other for arm_write_memory
  Elf32_Word* code = readCode(elfI, e, sectionNo, ENDIAN_LITTLE);

  arm_write_memory(simulator, currentSect.sh_addr, code, currentSect.sh_size);
}

void setInitalPc(arm_simulator_data_t simulator, ElfImageConstP elfI)
{
  arm_write_register(simulator, 15, elfI->hdr.e_entry);
}

void run(char* hostname, char* servicename, char const* filename)
{
  arm_simulator_data_t simulator;
  simulator = arm_connect(hostname, servicename);
  ElfFile   e = elfOpen(filename);
  ElfImage  elfi;
  ElfImageP elfIp = &elfi;
  initElfImage(elfIp);
  if (parseElf(elfIp, e))
  {
    addSection(simulator, elfIp, e, getSectionIdFromStr(elfIp, ".text"));
    addSection(simulator, elfIp, e, getSectionIdFromStr(elfIp, ".data"));
    setInitalPc(simulator, elfIp);
    arm_run(simulator);
  }
  else
  {
    printf("parsingError");
  }
}

int main(int argc, char* argv[])
{
  int   opt;
  char *hostname, *servicename;

  struct option longopts[] = {{"debug", required_argument, NULL, 'd'},
                              {"host", required_argument, NULL, 'H'},
                              {"service", required_argument, NULL, 'S'},
                              {"help", no_argument, NULL, 'h'},
                              {NULL, 0, NULL, 0}};

  hostname = NULL;
  servicename = NULL;
  while ((opt = getopt_long(argc, argv, "S:H:d:h", longopts, NULL)) != -1)
  {
    switch (opt)
    {
    case 'H':
      hostname = optarg;
      break;
    case 'S':
      servicename = optarg;
      break;
    case 'h':
      usage(argv[0]);
      exit(0);
    case 'd':
      add_debug_to(optarg);
      break;
    default:
      fprintf(stderr, "Unrecognized option %c\n", opt);
      usage(argv[0]);
      exit(1);
    }
  }

  char const* fileName = argv[optind];
}
