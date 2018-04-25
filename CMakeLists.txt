cmake_minimum_required(VERSION 3.3)
set(BEEHIVE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(LINKER_SCRIPT_DIR ${BEEHIVE_ROOT}/contrib/libopencm3/lib)
set(CMAKE_TOOLCHAIN_FILE "${BEEHIVE_ROOT}/cmake/stm32-toolchain.cmake")

project(BEEHIVE C CXX)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(contrib)

add_subdirectory(bee)
