# Custom ELF Loader intended for pedagogic use
![Travis](https://travis-ci.org/FederAndInk/ProjetS5.svg?branch=master)
[![codecov](https://codecov.io/gh/FederAndInk/ProjetS5/branch/master/graph/badge.svg)](https://codecov.io/gh/FederAndInk/ProjetS5)

## Telecharger

```bash
git clone --recursive https://github.com/FederAndInk/ProjetS5.git
# ou initialiser les submodules:
git submodules init
git submodules update
```

## Pour compiler:
```
./compile.sh
```
lance les tests à la fin

## Executer
```
build/src/<program>
```

## Tests
```
./launchTest.sh
```
0. **test1** (test1.c test1_include.{ch}): programme compilé avec la toolchain ARM avec `compileTest1.sh`
1. **testElfReader** test du lecteur de fichier elf utilise:
    - test1.o (généré par `compileTest1.sh`)
    - test1 (potentiellement ?)

## To do:
- [ ] Affichage de l’en-tête
- [ ] Affichage de la table des sections et des détails relatifs à chaque section
- [ ] Affichage du contenu d’une section
- [ ] Affichage de la table des symboles et des détails relatifs à chaque symbole
- [ ] Affichage des tables de réimplantation et des détails relatifs à chaque entrée
- [ ] Renumérotation des sections
- [ ] Correction des symboles
- [ ] Réimplantations de type R_ARM_ABS*
- [ ] Réimplantation de type R_ARM_JUMP24 et R ARM CALL
- [ ] Interfaçage avec le simulateur ARM
- [ ] Exécution à l’aide du simulateur arm-eabi-run.

## Docs:
- [Elf reading](https://wiki.osdev.org/ELF_Tutorial)
- [Elf structure](https://wiki.osdev.org/ELF)
