#include "ElfIO.h"
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
void elfOpenIn(ElfFile f, ElfMode m);

void elfOpenIn(ElfFile f, ElfMode m)
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

bool isElf(ElfFile f)
{
  return f != NULL;
}

bool elfIsSameEndianess(ElfFile f)
{
  return (f->eiData == ELFDATA2MSB && is_big_endian()) ||
         (f->eiData == ELFDATA2LSB && !is_big_endian());
}

void __fixEndianess(ElfFile f, void* ptr, size_t size)
{
  if (!elfIsSameEndianess(f))
  {
    reverseEndianess(ptr, size);
  }
}

#define fixEndianess(elf, e) __fixEndianess(elf, &e, sizeof(e))

void writeElfHeader(Elf32_Ehdr* eHdr, ElfFile dest)
{
  elfGoTo(dest, 0);

  for (size_t i = 0; i < EI_NIDENT; i++)
  {
    elfWriteUC(dest, eHdr->e_ident[i]);
  }

  // to update after changing type of elf (REL to EXEC)
  elfWrite16(dest, eHdr->e_type);

  elfWrite16(dest, eHdr->e_machine);
  elfWrite32(dest, eHdr->e_version);
  elfWrite32(dest, eHdr->e_entry);
  elfWrite32(dest, eHdr->e_phoff);

  // to update after moving sh
  elfWrite32(dest, eHdr->e_shoff);

  elfWrite32(dest, eHdr->e_flags);
  elfWrite16(dest, eHdr->e_ehsize);
  elfWrite16(dest, eHdr->e_phentsize);
  elfWrite16(dest, eHdr->e_phnum);
  elfWrite16(dest, eHdr->e_shentsize);

  // to update after deleting/adding section(s)
  elfWrite16(dest, eHdr->e_shnum);

  // to update after deleting/adding section(s)
  elfWrite16(dest, eHdr->e_shstrndx);
}

ElfFile elfCreate(Elf32_Ehdr* eHdr, char const* fileName)
{
  ElfFile res = NULL;
  res = (ElfFile)malloc(sizeof(struct ElfFile_t));
  if (res)
  {
    res->fileName = fileName;
    res->f = fopen(res->fileName, "wb+");
    if (!res->f)
    {
      perror("Erreur d'ouverture de fichier");
      free(res);
      res = NULL;
    }
    else
    {
      res->openMode = WRITE;
      res->eiData = eHdr->e_ident[EI_DATA];
      writeElfHeader(eHdr, res);
    }
  }

  return res;
}

ElfFile elfOpen(char const* fileName)
{
  ElfFile res = NULL;
  res = (ElfFile)malloc(sizeof(struct ElfFile_t));
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

Elf32_Word elfRead32(ElfFile f)
{
  elfOpenIn(f, READ);
  Elf32_Word res = 0;
  fread(&res, sizeof(res), 1, f->f);
  fixEndianess(f, res);
  return res;
}

Elf32_Half elfRead16(ElfFile f)
{
  elfOpenIn(f, READ);
  Elf32_Half res = 0;
  fread(&res, sizeof(res), 1, f->f);
  fixEndianess(f, res);
  return res;
}

unsigned char elfReadUC(ElfFile f)
{
  elfOpenIn(f, READ);
  unsigned char res = 0;
  fread(&res, sizeof(res), 1, f->f);
  return res;
}

void elfWrite32(ElfFile f, Elf32_Word e)
{
  elfOpenIn(f, WRITE);
  fixEndianess(f, e);
  fwrite(&e, sizeof(e), 1, f->f);
}

void elfWrite16(ElfFile f, Elf32_Half e)
{
  elfOpenIn(f, WRITE);
  fixEndianess(f, e);
  fwrite(&e, sizeof(e), 1, f->f);
}

void elfWriteUC(ElfFile f, unsigned char e)
{
  elfOpenIn(f, WRITE);
  fwrite(&e, sizeof(e), 1, f->f);
}

void elfGoTo(ElfFile f, size_t to)
{
  fseek(f->f, to, SEEK_SET);
}

void elfGoToRel(ElfFile f, size_t offset)
{
  fseek(f->f, offset, SEEK_CUR);
}

void elfGoToEnd(ElfFile f)
{
  fseek(f->f, 0, SEEK_END);
}

long elfTell(ElfFile f)
{
  return ftell(f->f);
}

void elfClose(ElfFile f)
{
  if (f)
  {
    fclose(f->f);
    free(f);
  }
}

unsigned char* elfReadUC_s(ElfFile f, size_t offset, size_t size)
{
  elfOpenIn(f, READ);
  elfGoTo(f, offset);

  unsigned char* str = (unsigned char*)malloc(sizeof(unsigned char) * size);

  fread(str, sizeof(unsigned char), size, f->f);

  return str;
}

void elfWriteUC_s(ElfFile f, size_t offset, size_t size, unsigned char* buf)
{
  elfOpenIn(f, WRITE);

  elfGoTo(f, offset);

  fwrite(buf, sizeof(unsigned char), size, f->f);
}
