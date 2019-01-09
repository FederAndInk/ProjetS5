#include "ElfIO.h"
#include "ElfImage.h"

/**
 * @brief delete relocations from elfI 
 * update section index in section headers and in symbols 
 * 
 * @param elfI 
 */
void deleteRelocationSections(ElfImageP elfI);

/**
 * @brief copy sections from src elf to dest elf
 * update offset in section header of elfI
 * 
 * @param elfI
 * @param dest
 * @param src 
 */
void copySections(ElfImageP elfI, ElfFile dest, ElfFile src);

/**
 * @brief write symbols of elfI to dest (at offset defined in section header in elfI)
 * get back to previous file offset after 
 * @param elfI 
 * @param dest 
 */
void writeSymbols(ElfImageP elfI, ElfFile dest);

/**
 * @brief write section headers (in elfI) to dest where the cursor is (in dest)
 * update offset in header of elfI (elfI->hdr)
 * 
 * @param elfI
 * @param dest
 */
void writeSectionHeaders(ElfImageP elfI, ElfFile dest);

void setNewOffsets(ElfImageConstP elfI, ElfFile dest);
