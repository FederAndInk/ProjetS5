#include "Elfstrtable.h"
#include "ElfReader.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


/*
    Function for get a string from a strtable

    e must be the elf file with the strtable you  want
    offset is the index of the beginning of your string from the beginning of the file.

    return the pointer to the string

*/
char* StrtableGetString(Elf e,uint32_t offset)
{
    elfGoTo(e,offset);

    char* str=malloc(sizeof(char)*50);
    int i=1;
    str[0]=elfReadUC(e);
    while (str[i-1]!='\0')
    {
        str[i]=elfReadUC(e);
        i++;
    }
    str=realloc(str,sizeof(char)*i);
    return str;
}
