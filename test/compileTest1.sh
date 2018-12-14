#!/bin/bash

arm-linux-gnueabi-gcc -mno-thumb-interwork -S test1.c
arm-linux-gnueabi-gcc -mno-thumb-interwork -S test1_include.c

arm-linux-gnueabi-as -o test1.o test1.s
arm-linux-gnueabi-as -o test1_include.o test1_include.s

arm-linux-gnueabi-ld -r -o test1Linked.o test1.o test1_include.o

arm-linux-gnueabi-gcc -o test1 test1Linked.o

echo "compilation finished"
