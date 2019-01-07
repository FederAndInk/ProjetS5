#pragma once

#include <elf.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef enum ElfMode
{
  NOT_OPENED,
  READ,
  WRITE
} ElfMode;

// TODO: Handle errors (R/W + goto)
typedef struct Elf_t
{
  char const*   fileName;
  FILE*         f;
  ElfMode       openMode;
  unsigned char eiData;
} * Elf;

bool isElf(Elf f);

/**
 * @brief indique si f à la meme endianess que la plateform courante
 *
 * @param f
 * @return true si f à la meme endianess que la plateform courante
 * @return false sinon
 */
bool elfIsSameEndianess(Elf f);

/**
 * @brief ouvre fileName en tant que fichier ELF
 * retourne NULL si fileName n'est pas un fichier elf (traité par isElf)
 *
 * @param fileName
 * @return Elf
 */
Elf elfOpen(char const* fileName);

/**
 * @brief lit 32 bits brut du fichier elf f
 *
 * change potentiellement le mode de f
 *
 * @param f
 * @return Elf32_Word
 */
Elf32_Word elfRead32(Elf f);

/**
 * @brief lit 16 bits brut du fichier elf f
 *
 * change potentiellement le mode de f
 *
 * @param f
 * @return Elf32_Half
 */
Elf32_Half elfRead16(Elf f);

/**
 * @brief lit un unsigned char du fichier elf f
 *
 * change potentiellement le mode de f
 *
 * @param f
 * @return Elf32_Half
 */
unsigned char elfReadUC(Elf f);

/**
 * @brief ecrit 32 bits (e) dans le fichier f à la position du curseur
 *
 * change potentiellement le mode de f
 *
 * @param f
 * @param e
 */
void elfWrite32(Elf f, Elf32_Word e);

/**
 * @brief ecrit 16 bits (e) dans le fichier f à la position du curseur
 *
 * change potentiellement le mode de f
 *
 * @param f
 * @param e
 */
void elfWrite16(Elf f, Elf32_Half e);

/**
 * @brief ecrit un unsigned char dans le fichier f à la position du curseur
 *
 * change potentiellement le mode de f
 *
 * @param f
 * @return Elf32_Half
 */
void elfWriteUC(Elf f, unsigned char e);

/**
 * @brief aller à l'octet to
 *
 * @param f
 * @param to
 */
void elfGoTo(Elf f, size_t to);

/**
 * @brief se deplace de offset octets
 *
 * @param f
 * @param offset
 */
void elfGoToRel(Elf f, size_t offset);

long elfTell(Elf f);

void elfClose(Elf f);

/**
 *  get a chunk of data of size size at offset
 *  @return a pointer to the chunk allocated onto the heap
 */
unsigned char* elfReadUC_s(Elf e, size_t offset, size_t size);

/**
 * @brief write a chunk of data of size size at offset
 * 
 */
void elfWriteUC_s(Elf e, size_t offset, size_t size, unsigned char* buf);
