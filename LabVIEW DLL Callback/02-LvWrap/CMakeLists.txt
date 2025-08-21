cmake_minimum_required(VERSION 3.24)
project(LvWrap C)

set(CMAKE_C_STANDARD 11)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include_directories("C:/Program Files (x86)/National Instruments/LabVIEW 2015/cintools")

add_library(LvWrap SHARED library.c)

target_link_libraries(LvWrap "C:/Program Files (x86)/National Instruments/LabVIEW 2015/cintools/labviewv.lib")
