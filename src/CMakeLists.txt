cmake_minimum_required(VERSION 3.0)
project(Elf VERSION 0.1.0)

add_compile_options($<$<CONFIG:Debug>:-Wall>
                    $<$<CONFIG:Debug>:-Wextra>)

add_compile_definitions($<$<CONFIG:Debug>:DEBUG>)

add_library(util util.c util.h)

add_library(ElfIO ./ElfIO.c ./ElfIO.h)
target_link_libraries(ElfIO util)

add_library(ElfString ElfString.c ElfString.h)

add_library(ElfImage ElfImage.c ElfImage.h)

add_library(ElfStringTable ElfStringTable.c
            ElfStringTable.h)
target_link_libraries(ElfStringTable ElfIO
                      ElfImage)

add_library(ElfParser ./ElfParser.c ./ElfParser.h)
target_link_libraries(ElfParser
                      ElfIO
                      ElfStringTable
                      ElfImage)

add_library(ElfPrinter ElfPrinter.c ElfPrinter.h)
target_link_libraries(ElfPrinter
                      ElfImage
                      ElfString
                      ElfStringTable
                      ElfIO)

add_library(ElfWriter ElfWriter.c ElfWriter.h)

add_executable(ElfLinker ElfLinker.c)

target_link_libraries(ElfLinker
                      ElfWriter
                      util
                      ElfImage
                      ElfParser
                      ElfStringTable
                      ElfIO)

add_executable(readelf readelf.c)

target_link_libraries(readelf
                      ElfPrinter
                      ElfParser
                      ElfIO)

add_library(ArmSimulator
            arm_simulator_interface.c
            arm_simulator_interface.h
            csapp.c
            csapp.h
            debug.c
            debug.h
            gdb_protocol.c
            gdb_protocol.h
            scanner.c
            scanner.h)
target_link_libraries(ArmSimulator util pthread m)

add_executable(ArmRunnerExample
               ./ARM_runner_example.c)
target_link_libraries(ArmRunnerExample
                      ArmSimulator)

add_executable(ArmRun ArmRun.c)
target_link_libraries(ArmRun
                      ArmSimulator
                      ElfImage
                      ElfIO
                      ElfStringTable
                      ElfParser)
