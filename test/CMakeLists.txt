cmake_minimum_required(VERSION 3.0)
project(test)

enable_testing()

configure_file(compileTest1.sh compileTest1.sh
               COPYONLY)
configure_file(test1.c test1.c COPYONLY)
configure_file(test1_include.c test1_include.c
               COPYONLY)
configure_file(test1_include.h test1_include.h
               COPYONLY)

add_custom_command(OUTPUT test1 test1.o
                          test1Linked.o
                   COMMAND ./compileTest1.sh
                   DEPENDS ./test1.c
                           ./test1_include.c
                           ./test1_include.h)
add_custom_target(RunCompileTest1 ALL
                  DEPENDS test1)

add_executable(TestElfReader testElfReader.c)

target_include_directories(
  TestElfReader
  PRIVATE "${CMAKE_SOURCE_DIR}/src/")

target_link_libraries(TestElfReader ElfReader)

add_test(TestElfReader TestElfReader
         ${CMAKE_CURRENT_BINARY_DIR}/test1.o)



