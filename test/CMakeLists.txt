cmake_minimum_required(VERSION 3.0)
project(test)

add_library(UnitTest UnitTest.c UnitTest.h)

add_executable(TestElfReader testElfReader.c)
add_executable(TestElfWriter testElfWriter.c)

target_include_directories(
  TestElfReader
  PRIVATE "${CMAKE_SOURCE_DIR}/src/")

target_include_directories(
  TestElfWriter
  PRIVATE "${CMAKE_SOURCE_DIR}/src/")

target_link_libraries(TestElfReader ElfIO
                      UnitTest)

target_link_libraries(TestElfWriter ElfIO
                      UnitTest)

# add_test( NAME TestElfHeader COMMAND
# "${CMAKE_BINARY_DIR}/src/ElfHeader"
# ${CMAKE_CURRENT_BINARY_DIR}/test1.o
# WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(
  NAME TestElfReader
  COMMAND
    TestElfReader
    ${CMAKE_CURRENT_LIST_DIR}/bin/ELFV1_LE_REL_ARM32.o
    ${CMAKE_CURRENT_LIST_DIR}/bin/ELFV1_BE_REL_ARM32.o
    ${CMAKE_CURRENT_LIST_DIR}/bin/NONELF.c
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(
  NAME TestElfWriter
  COMMAND
    TestElfWriter
    ${CMAKE_CURRENT_LIST_DIR}/bin/ELFV1_LE_REL_ARM32.o
    ${CMAKE_CURRENT_LIST_DIR}/bin/ELFV1_BE_REL_ARM32.o
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
