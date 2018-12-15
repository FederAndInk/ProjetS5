#include "ElfReader.h"
#include "util.h"
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

bool isElf(Elf f)
{
  return f != NULL;
}

bool elfIsSameIndianess(Elf f)
{
  return (f->eiData == ELFDATA2MSB && is_big_endian()) ||
         (f->eiData == ELFDATA2LSB && !is_big_endian());
}

Elf elfOpen(char const* fileName) {}

uint32_t elfRead32(Elf f) {}

uint16_t elfRead16(Elf f) {}

unsigned char elfReadUC(Elf f) {}

void elfWrite32(Elf f, uint32_t e) {}

void elfWrite16(Elf f, uint16_t e) {}

void elfWriteUC(Elf f, unsigned char e) {}

void elfGoTo(Elf f, size_t to) {}

void elfGoToRel(Elf f, size_t offset) {}

void close(Elf f) {}
