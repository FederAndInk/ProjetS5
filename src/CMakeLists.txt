cmake_minimum_required(VERSION 3.10)
project(Elf VERSION 0.1.0)

add_compile_options($<$<CONFIG:Debug>:-Wall>
                    $<$<CONFIG:Debug>:-Wextra>)

add_library(ElfReader
            ./ElfReader.c
            ./ElfReader.h
            ./Elf.h)

add_library(ElfParser ./ElfParser.c ./ElfReader.h)
target_link_libraries(ElfParser ElfReader)

add_executable(ElfHeader ElfHeader.c)
target_link_libraries(ElfHeader ElfParser
                      ElfReader)
