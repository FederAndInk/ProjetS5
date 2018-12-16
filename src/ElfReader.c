#include "ElfReader.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

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

Elf elfOpen(char const* fileName) {
  Elf res = NULL;
  res = malloc(sizeof(Elf));
  if(res != NULL)
  {
    res->fileName = fileName ;
    res->f = fopen(res->fileName, "rb");
    if(res->f == NULL)
    {
      fprintf(stderr, "Erreur d'ouverture de fichier.\n");
    }
    res->openMode = READ;
  }
  return res;
}

uint32_t elfRead32(Elf f) {
  uint32_t res = 0;
  uint8_t c ;
  int len = 4;
  while(len--)
  {
    res <<= 8;
    c = fgetc(f->f);
    res = c | res ;
  }
  return res;
}

uint16_t elfRead16(Elf f) {
  uint32_t res = 0;
  uint8_t c ;
  int len = 2;
  while(len--)
  {
    res <<= 8;
    c = fgetc(f->f);
    res = c | res ;
  }
  return res;
}

unsigned char elfReadUC(Elf f) {
  unsigned char res = 0;
  return res;
}

void elfWrite32(Elf f, uint32_t e) {}

void elfWrite16(Elf f, uint16_t e) {}

void elfWriteUC(Elf f, unsigned char e) {}

void elfGoTo(Elf f, size_t to) {}

void elfGoToRel(Elf f, size_t offset) {}

void close(Elf f) {
  fclose(f->f);
}
