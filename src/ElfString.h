#pragma once

typedef struct
{
  long        i;
  char const* name;
} ElfType;

extern char const ELF_UNKNOWN[];

extern ElfType sht[];
extern ElfType stb[];
extern ElfType stt[];
extern ElfType stv[];

/**
 * @return the string for the type i in t
 * or NULL if the type hasn't string associated with it
 */
char const* getElfType(ElfType t[], long i);
