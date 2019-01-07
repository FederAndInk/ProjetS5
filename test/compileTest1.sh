#!/bin/bash

if [ `command -v arm-linux-gnueabihf-gcc` ]; then
    TOOLCHAIN=arm-linux-gnueabihf-
    
elif [ `command -v arm-linux-gnueabi-gcc` ]; then
    TOOLCHAIN=arm-linux-gnueabi-
elif [ `command -v arm-eabi-gcc` ]; then
    TOOLCHAIN=arm-eabi-
else
    echo "no toolchain found"
    exit 1
fi
# ADD An elif block for other toolchain

C_FLAGS=""
LD_FLAGS="-nostdlib --entry main -n --section-start .text=0x20 --section-start .data=0x2800"
GCC_LD_FLAGS="-no-pie -nostdlib --entry main -n -Wl,--section-start,.text=0x20,--section-start,.data=0x2800"

${TOOLCHAIN}gcc -mno-thumb-interwork -S test1.c
${TOOLCHAIN}gcc -mno-thumb-interwork -S test1_include.c

${TOOLCHAIN}as ${C_FLAGS} -o test1.o test1.s
${TOOLCHAIN}as ${C_FLAGS} -o test1_include.o test1_include.s

${TOOLCHAIN}ld ${LD_FLAGS} -r -o test1Linked.o test1.o test1_include.o

${TOOLCHAIN}gcc ${GCC_LD_FLAGS} -o test1 test1Linked.o

echo "compilation finished"
