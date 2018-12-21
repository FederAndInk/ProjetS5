# How to implement properly relocation R_ARM_CALL on ARM

I have read the [Elf for ARM architecture](http://infocenter.arm.com/help/topic/com.arm.doc.ihi0044f/IHI0044F_aaelf.pdf) and other docs but I couldn't find how to properly implement R_ARM_CALL relocation described at page 25 to page 33.
Could you guide me or point me to some more documentation ?

I know how to get a part of the relocation (rel not rela), as said in the doc:

- First I compute the value of A: A = sign_extend(insn[23:0] << 2)
- I compute the relocation: X = S + A - P (I don't care of thumb instructions, so I don't need T)
- I apply the mask: result_mask(X) = X & 0x03 FF FF FE

And then as it is in the doc I have to "see call and jump relocations"
but in this section there is nothing about writing back the result in the instruction.

I have made some tests and found when comparing a ".o" with the executable to see how the linker (arm-linux-eabi-gcc) implement this relocation that he just shift right of 2, so it give me something like:
insn[23:0] = result_mask(X) >> 2

but I would like to found that on the documentation to do a correct implementation.

I have compiled with `-mno-thumb-interwork -no-pie -nostdlib --entry main -n -Wl,--section-start,.text=0x20,--section-start,.data=0x2800,-EB`

I put .text at 0x20 and .data at 0x2800

relocation of ".o"

```
Relocation section '.rel.text' at offset 0x1e0 contains 10 entries:
 Offset     Info    Type            Sym.Value  Sym. Name
00000004  0000021c R_ARM_CALL        00000000   .data
00000008  00000e1c R_ARM_CALL        00000018   data_global
0000000c  0000021d R_ARM_JUMP24      00000000   .data
```

.text of ".o"

```
Hex dump of section '.text':
 NOTE: This section has relocations against it, but these have NOT been applied to this dump.
  0x00000000 e1a00000 eb000000 ebfffffe ea000000 ................
  0x00000010 eafffffe eb000003 ebfffffe ea000001 ................
  0x00000020 eafffffe e1a00000 e1a0f00e 00000028 ...............(
  0x00000030 00000000 00000008 00000000          ............
```

.text of executable

```
Hex dump of section '.text':
  0x00000020 e1a00000 eb0009f7 eb0009fa ea0009f5 ................
  0x00000030 ea0009f8 eb000003 ebfffff9 ea000001 ................
  0x00000040 eafffff7 e1a00000 e1a0f00e 00000048 ...............H
  0x00000050 00000024 00002808 00002818          ...$..(...(.
```

And if we try to compute the first relocation:
S = 0x2800 (.data)
P = 0x20 + 0x4 = 0x24
A = sign_extend(0xeb 00 00 00 [23:0] << 2) = 0x00 00 00 00

X = S + A - P = 0x27DC
mask = X & 0x03 FF FF FE = 0x27DC

and I tried a right shift of 2:
0x27DC >> 2 = 0x9f7

the value that we have to find according to the linker: **eb0009f7**

it also works for the second relocation when A isn't null.
