#!/bin/bash

if [ `command -v arm-linux-gnueabihf-gcc` ]; then
    TOOLCHAIN=arm-linux-gnueabihf-
    
elif [ `command -v arm-linux-gnueabi-gcc` ]; then
    TOOLCHAIN=arm-linux-gnueabi-
elif [ `command -v arm-eabi-gcc` ]; then
    TOOLCHAIN=arm-eabi-
fi
# ADD An elif block for other toolchain



${TOOLCHAIN}gcc -mno-thumb-interwork -S test1.c
${TOOLCHAIN}gcc -mno-thumb-interwork -S test1_include.c

${TOOLCHAIN}as -o test1.o test1.s
${TOOLCHAIN}as -o test1_include.o test1_include.s

${TOOLCHAIN}ld -r -o test1Linked.o test1.o test1_include.o

${TOOLCHAIN}gcc --static -o test1 test1Linked.o

echo "compilation finished"
