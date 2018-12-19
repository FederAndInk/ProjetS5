#pragma once

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
 * @return uint32_t
 */
uint32_t elfRead32(Elf f);

/**
 * @brief lit 16 bits brut du fichier elf f
 *
 * change potentiellement le mode de f
 *
 * @param f
 * @return uint16_t
 */
uint16_t elfRead16(Elf f);

/**
 * @brief lit un unsigned char du fichier elf f
 *
 * change potentiellement le mode de f
 *
 * @param f
 * @return uint16_t
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
void elfWrite32(Elf f, uint32_t e);

/**
 * @brief ecrit 16 bits (e) dans le fichier f à la position du curseur
 *
 * change potentiellement le mode de f
 *
 * @param f
 * @param e
 */
void elfWrite16(Elf f, uint16_t e);

/**
 * @brief ecrit un unsigned char dans le fichier f à la position du curseur
 *
 * change potentiellement le mode de f
 *
 * @param f
 * @return uint16_t
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

void elfClose(Elf f);

/*
    Function for get a string from a strtable

    e must be the elf file with the strtable you  want
    offset is the index of the beginning of your string from the beginning of the file.

    return the pointer to the string

*/
char* StrtableGetString(Elf e,uint32_t offset);
