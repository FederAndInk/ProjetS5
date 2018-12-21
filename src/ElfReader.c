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

uint32_t elfRead32(Elf f)
{
  elfOpenIn(f, READ);
  uint32_t res = 0;
  fread(&res, sizeof(res), 1, f->f);
  fixEndianess(f, res);
  return res;
}

uint16_t elfRead16(Elf f)
{
  elfOpenIn(f, READ);
  uint16_t res = 0;
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

void elfWrite32(Elf f, uint32_t e)
{
  elfOpenIn(f, WRITE);
  fixEndianess(f, e);
  fwrite(&e, sizeof(e), 1, f->f);
}

void elfWrite16(Elf f, uint16_t e)
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

/*
    Function for getting a string from a strtable

    e must be the elf file with the strtable you  want
    offset is the index of the beginning of your string from the beginning of the file.

    return the pointer to the string that must be free

*/
char* StrtableGetString(Elf e,uint32_t offset)
{
    elfGoTo(e,offset);

    char* str=NULL;
    size_t n;
    getdelim(&str,&n,'\0',e->f);

    return str;
}
