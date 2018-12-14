#include "ElfReader.h"
#include <stdio.h>

// Implementation function

/**
 * @brief open f in mode m
 * do nothing if f is already in mode m
 * 
 * @param f 
 * @param m 
 */
void ElfOpenIn(Elf f, ElfMode m);

void ElfOpenIn(Elf f, ElfMode m) {}

// End Implementation function

bool isElf(char const* file)
{
  FILE* f = fopen(file, "r");
  fgetc(f);
  char elf[4] = {0};
  elf[0] = fgetc(f);
  elf[1] = fgetc(f);
  elf[2] = fgetc(f);
  return elf[0] == 'E' && elf[1] == 'L' && elf[2] == 'F';
}

Elf elfOpen(char const* fileName) {}

uint32_t elfRead32(Elf f) {}

uint16_t elfRead16(Elf f) {}

void elfWrite32(Elf f, uint32_t e) {}

void elfWrite16(Elf f, uint16_t e) {}

void elfGoTo(Elf f, size_t to) {}

void elfGoToRel(Elf f, size_t offset) {}

void close(Elf f) {}
