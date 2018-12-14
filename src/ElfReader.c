#include "ElfReader.h"

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

Elf elfOpen(char const* fileName) {}

uint32_t elfRead32(Elf f) {}

uint16_t elfRead16(Elf f) {}

void elfWrite32(Elf f, uint32_t e) {}

void elfWrite16(Elf f, uint16_t e) {}

void elfGoTo(Elf f, size_t to) {}

void elfGoToRel(Elf f, size_t offset) {}

void close(Elf f) {}
