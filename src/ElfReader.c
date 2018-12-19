#include "ElfReader.h"
#include "util.h"
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implementation function

/**
 * @brief open f in mode m
 * do nothing if f is already in mode m
 *
 * @param f
 * @param m
 */
void elfOpenIn(Elf f, ElfMode m);

void elfOpenIn(Elf f, ElfMode m)
{
  if (f->openMode != m)
  {
    if (m == WRITE)
    {
      fseek(f->f, 0, SEEK_CUR);
    }
    else if (m == READ)
    {
      fflush(f->f);
    }
  }
}

// End Implementation function

bool isElf(Elf f)
{
  return f != NULL;
}

bool elfIsSameEndianess(Elf f)
{
  return (f->eiData == ELFDATA2MSB && is_big_endian()) ||
         (f->eiData == ELFDATA2LSB && !is_big_endian());
}

void __fixEndianess(Elf f, void* ptr, size_t size)
{
  if (!elfIsSameEndianess(f))
  {
    reverseEndianess(ptr, size);
  }
}

#define fixEndianess(elf, e) __fixEndianess(elf, &e, sizeof(e))

Elf elfOpen(char const* fileName)
{
  Elf res = NULL;
  res = (Elf)malloc(sizeof(struct Elf_t));
  if (res)
  {
    res->fileName = fileName;
    res->f = fopen(res->fileName, "rb+");
    if (!res->f)
    {
      perror("Erreur d'ouverture de fichier");
      free(res);
      res = NULL;
    }
    else
    {
      res->openMode = READ;
      unsigned char magic[6];
      fread(magic, sizeof(*magic), 6, res->f);

      if (strncmp((char*)magic, ELFMAG, 4) == 0)
      {
        res->eiData = magic[EI_DATA];
        rewind(res->f);
      }
      else
      {
        fputs("Erreur le fichier n'est pas au format ELF\n", stderr);
        fclose(res->f);
        free(res);
        res = NULL;
      }
    }
  }
  return res;
}

Elf32_Word elfRead32(Elf f)
{
  elfOpenIn(f, READ);
  Elf32_Word res = 0;
  fread(&res, sizeof(res), 1, f->f);
  fixEndianess(f, res);
  return res;
}

Elf32_Half elfRead16(Elf f)
{
  elfOpenIn(f, READ);
  Elf32_Half res = 0;
  fread(&res, sizeof(res), 1, f->f);
  fixEndianess(f, res);
  return res;
}

unsigned char elfReadUC(Elf f)
{
  elfOpenIn(f, READ);
  unsigned char res = 0;
  fread(&res, sizeof(res), 1, f->f);
  return res;
}

void elfWrite32(Elf f, Elf32_Word e)
{
  elfOpenIn(f, WRITE);
  fixEndianess(f, e);
  fwrite(&e, sizeof(e), 1, f->f);
}

void elfWrite16(Elf f, Elf32_Half e)
{
  elfOpenIn(f, WRITE);
  fixEndianess(f, e);
  fwrite(&e, sizeof(e), 1, f->f);
}

void elfWriteUC(Elf f, unsigned char e)
{
  elfOpenIn(f, WRITE);
  fwrite(&e, sizeof(e), 1, f->f);
}

void elfGoTo(Elf f, size_t to)
{
  fseek(f->f, to, SEEK_SET);
}

void elfGoToRel(Elf f, size_t offset)
{
  fseek(f->f, offset, SEEK_CUR);
}

void elfClose(Elf f)
{
  fclose(f->f);
  free(f);
}

unsigned char* elfReadUC_s(Elf e, size_t offset, size_t size)
{
  elfGoTo(e, offset);

  unsigned char* str = (unsigned char*)malloc(sizeof(unsigned char) * size);

  fread(str, sizeof(unsigned char), size, e->f);

  return str;
}
