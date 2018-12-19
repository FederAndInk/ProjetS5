cmake_minimum_required(VERSION 3.0)
project(Elf VERSION 0.1.0)

add_compile_options($<$<CONFIG:Debug>:-Wall>
                    $<$<CONFIG:Debug>:-Wextra>)

add_library(util util.c util.h)

add_library(ElfReader
            ./ElfReader.c
            ./ElfReader.h
            ./Elf.h)
target_link_libraries(ElfReader util)

add_library(ElfParser ./ElfParser.c ./ElfReader.h)
target_link_libraries(ElfParser ElfReader)

add_executable(ElfHeader ElfHeader.c)
target_link_libraries(ElfHeader ElfParser
                      ElfReader)

add_library(Elfstrtable Elfstrtable.c Elfstrtable.h)

add_executable(ElfSectionheader ElfSectionheader.c)
target_link_libraries(ElfSectionheader ElfParser
                      ElfReader Elfstrtable)