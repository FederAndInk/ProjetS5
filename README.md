# Custom ELF Loader intended for pedagogic use

[![Travis](https://travis-ci.org/FederAndInk/ProjetS5.svg?branch=master)](https://travis-ci.org/FederAndInk/ProjetS5)
[![codecov](https://codecov.io/gh/FederAndInk/ProjetS5/branch/master/graph/badge.svg)](https://codecov.io/gh/FederAndInk/ProjetS5)

## Telecharger
 **testElfReader**
```bash
git clone --recursive https://github.com/FederAndInk/ProjetS5.git
# ou initialiser les submodules:
git submodules init
git submodules update
```

## Pour compiler

```bash
./compile.sh
```

lance les tests à la fin

## Executer

```bash
build/src/<program>
```

## To do

- [x] Affichage de l’en-tête
- [x] Affichage de la table des sections et des détails relatifs à chaque section
- [ ] Affichage du contenu d’une section
- [ ] Affichage de la table des symboles et des détails relatifs à chaque symbole
- [ ] Affichage des tables de réimplantation et des détails relatifs à chaque entrée
- [ ] Renumérotation des sections
- [ ] Correction des symboles
- [ ] Réimplantations de type R_ARM_ABS*
- [ ] Réimplantation de type R_ARM_JUMP24 et R ARM CALL
- [ ] Interfaçage avec le simulateur ARM
- [ ] Exécution à l’aide du simulateur arm-eabi-run.

## Tests

```bash
./launchTest.sh
./testReadElf.sh
```

0. **launchTest.sh** (test1.c test1_include.{ch}): programme compilé avec la toolchain ARM avec `compileTest1.sh`
1. **testReadElf.sh** test du lecteur de fichier elf utilise:
    - test1.o (généré par `compileTest1.sh`)
    - test1 (potentiellement ?)

## Docs

- [Elf reading](https://wiki.osdev.org/ELF_Tutorial)
- [Elf structure](https://wiki.osdev.org/ELF)
