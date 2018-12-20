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

mkdir -p build
cd build

C_FLAGS="-mbig-endian"
LD_FLAGS="-nostdlib --entry main -n --section-start .text=0x20 --section-start .data=0x2800 -EB"
GCC_LD_FLAGS="-no-pie -nostdlib --entry main -mno-thumb-interwork -n -Wl,--section-start,.text=0x20,--section-start,.data=0x2800,-EB"

${TOOLCHAIN}as ${C_FLAGS} -o ex1.o ../example1.s
${TOOLCHAIN}as ${C_FLAGS} -o ex2.o ../example2.s
${TOOLCHAIN}as ${C_FLAGS} -o ex3.o ../example3.s
${TOOLCHAIN}as ${C_FLAGS} -o ex4.o ../example4.s

${TOOLCHAIN}ld ${LD_FLAGS} -r -o ex1Linked.o ex1.o
${TOOLCHAIN}ld ${LD_FLAGS} -r -o ex2Linked.o ex2.o
${TOOLCHAIN}ld ${LD_FLAGS} -r -o ex3Linked.o ex3.o
${TOOLCHAIN}ld ${LD_FLAGS} -r -o ex4Linked.o ex4.o

${TOOLCHAIN}gcc ${GCC_LD_FLAGS} -o ex1 ex1Linked.o
${TOOLCHAIN}gcc ${GCC_LD_FLAGS} -o ex2 ex2Linked.o
${TOOLCHAIN}gcc ${GCC_LD_FLAGS} -o ex3 ex3Linked.o
${TOOLCHAIN}gcc ${GCC_LD_FLAGS} -o ex4 ex4Linked.o

echo "compilation finished"
