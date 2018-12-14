#pragma once
#include "ElfReader.h"
#include <linux/elf.h>

void parseHeader(Elf e, Elf32_Ehdr* hdr);
