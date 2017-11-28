cmake_minimum_required(VERSION 3.3)
set(DRONE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/stm32-clang-toolchain.cmake")

project(DRONE_HAT C CXX)
set (CMAKE_CXX_STANDARD 11)

add_subdirectory(contrib)

add_subdirectory(drone_hat)