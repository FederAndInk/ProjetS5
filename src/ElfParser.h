#pragma once
#include "ElfReader.h"
#include <linux/elf.h>

/**
 * @brief parse the header of the elf file e
 * if e is not of class ELF32 the parsing stops before arch specific fields
 * e_ident is valid
 * @param e 
 * @param hdr 
 */
void parseHeader(Elf e, Elf32_Ehdr* hdr);
