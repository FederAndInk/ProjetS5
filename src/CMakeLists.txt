cmake_minimum_required(VERSION 3.0)
project(Elf VERSION 0.1.0)

add_compile_options($<$<CONFIG:Debug>:-Wall>
                    $<$<CONFIG:Debug>:-Wextra>)

add_library(util util.c util.h)

add_library(ElfReader ./ElfReader.c ./ElfReader.h)
target_link_libraries(ElfReader util)

add_library(ElfString ElfString.c ElfString.h)

add_library(Elf Elf.c Elf.h)

add_library(ElfStringTable ElfStringTable.c
            ElfStringTable.h)
target_link_libraries(ElfStringTable ElfReader
                      Elf)

add_library(ElfParser ./ElfParser.c ./ElfParser.h)
target_link_libraries(ElfParser
                      ElfReader
                      ElfStringTable
                      Elf)

add_executable(ElfHeader ElfHeader.c)
target_link_libraries(ElfHeader
                      ElfParser
                      ElfReader
                      Elf)

add_executable(ElfSectionheader
               ElfSectionheader.c)
target_link_libraries(ElfSectionheader
                      ElfParser
                      ElfReader
                      ElfString
                      ElfStringTable
                      Elf)
                      
add_executable(ElfSymbol ElfSymbol.c)
target_link_libraries(ElfSymbol
                      ElfParser
                      ElfReader
                      ElfString
                      ElfStringTable
                      Elf)
add_executable(ElfRelocation ElfRelocation.c)
target_link_libraries(ElfRelocation
                      ElfParser
                      ElfReader
                      ElfString
                      ElfStringTable
                      Elf)
