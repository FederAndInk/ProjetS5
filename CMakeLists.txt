cmake_minimum_required(VERSION 3.0)
project(ElfProject VERSION 0.1.0 LANGUAGES C)

enable_testing()

add_compile_options(
  --coverage $<$<CONFIG:Debug>:-g>
  $<$<CONFIG:Debug>:-fsanitize=undefined,address,bounds>
  )

link_libraries(
  --coverage
  $<$<CONFIG:Debug>:-fsanitize=undefined,address,bounds>
  )

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(src)

add_subdirectory(test)
