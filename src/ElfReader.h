#pragma once

#include <stdint.h>
#include <stdio.h>

typedef enum ElfMode
{
  NOT_OPENED,
  READ,
  WRITE
} ElfMode;

typedef struct Elf
{
  char const* fileName;
  FILE*       f;
  ElfMode     openMode;
} * Elf;

Elf elfOpen(char const* fileName);

// TODO : Voir comment prendre en compte l'endianess (when read and write), s' il faut la prendre en compte

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

void close(Elf);
