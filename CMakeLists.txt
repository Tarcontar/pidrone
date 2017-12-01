cmake_minimum_required(VERSION 3.3)
set(DRONE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(LINKER_SCRIPT ${DRONE_ROOT}/contrib/libopencm3/lib/libopencm3_stm32f3.ld)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/stm32-toolchain.cmake")

project(DRONE_HAT C CXX)
set (CMAKE_CXX_STANDARD 11)

add_subdirectory(contrib)

add_subdirectory(drone_hat)