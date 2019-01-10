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

typedef enum
{
  /// endianess of the file
  ENDIAN_DEFAULT,
  /// endianess of the system
  ENDIAN_SYSTEM,
  /// little endian
  ENDIAN_LITTLE,
  /// big endian
  ENDIAN_BIG
} Endianness;

// TODO: Handle errors (R/W + goto)
typedef struct ElfFile_t
{
  char const*   fileName;
  FILE*         f;
  ElfMode       openMode;
  unsigned char eiData;
} * ElfFile;

bool isElf(ElfFile f);

/**
 * @brief indique si f à la meme endianess que la plateform courante
 *
 * @pre isElf(f) == true
 * @param f
 * @return true si f à la meme endianess que la plateform courante
 * @return false sinon
 */
bool elfIsSameEndianess(ElfFile f);

bool elfIsEndianness(ElfFile f, Endianness end);

void writeElfHeader(Elf32_Ehdr const* eHdr, ElfFile dest);

/**
 * @brief create a new elf file with header eHdr written
 * 
 * @param fileName 
 * @return ElfFile 
 */
ElfFile elfCreate(Elf32_Ehdr const* eHdr, char const* fileName);

/**
 * @brief ouvre fileName en tant que fichier ELF
 * retourne NULL si fileName n'est pas un fichier elf (traité par isElf)
 *
 * @param fileName
 * @return Elf
 */
ElfFile elfOpen(char const* fileName);

/**
 * @brief lit 32 bits brut du fichier elf f
 *
 * change potentiellement le mode de f
 *
 * @pre isElf(f) == true
 * @param f
 * @return Elf32_Word
 */
Elf32_Word elfRead32(ElfFile f);

/**
 * @brief lit 16 bits brut du fichier elf f
 *
 * change potentiellement le mode de f
 *
 * @param f
 * @return Elf32_Half
 */
Elf32_Half elfRead16(ElfFile f);

/**
 * @brief lit un unsigned char du fichier elf f
 *
 * change potentiellement le mode de f
 *
 * @pre isElf(f) == true
 * @param f
 * @return Elf32_Half
 */
unsigned char elfReadUC(ElfFile f);

/**
 * @brief ecrit 32 bits (e) dans le fichier f à la position du curseur
 *
 * change potentiellement le mode de f
 *
 * @pre isElf(f) == true
 * @param f
 * @param e
 */
void elfWrite32(ElfFile f, Elf32_Word e);

/**
 * @brief ecrit 16 bits (e) dans le fichier f à la position du curseur
 *
 * change potentiellement le mode de f
 *
 * @pre isElf(f) == true
 * @param f
 * @param e
 */
void elfWrite16(ElfFile f, Elf32_Half e);

/**
 * @brief ecrit un unsigned char dans le fichier f à la position du curseur
 *
 * change potentiellement le mode de f
 *
 * @pre isElf(f) == true
 * @param f
 * @return Elf32_Half
 */
void elfWriteUC(ElfFile f, unsigned char e);

/**
 * @brief aller à l'octet to
 *
 * @pre isElf(f) == true
 * @param f
 * @param to
 */
void elfGoTo(ElfFile f, long to);

/**
 * @brief se deplace de offset octets
 *
 * @pre isElf(f) == true
 * @param f
 * @param offset
 */
void elfGoToRel(ElfFile f, long offset);

void elfGoToEnd(ElfFile f);

long elfTell(ElfFile f);

/**
 * @brief close the elf f
 * 
 * @pre isElf(f) == true
 * @param f 
 */
void elfClose(ElfFile f);

/**
 * get a chunk of data of size size at offset
 * @pre isElf(f) == true
 * @return a pointer to the chunk allocated onto the heap
 */
unsigned char* elfReadUC_s(ElfFile f, size_t offset, size_t size);

/**
 * @brief write a chunk of data of size size at offset
 * 
 */
void elfWriteUC_s(ElfFile f, size_t offset, size_t size, unsigned char* buf);
