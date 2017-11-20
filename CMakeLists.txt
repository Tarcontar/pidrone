cmake_minimum_required(VERSION 3.3)
set(DRONE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(AVR_MCU "atmega328p" CACHE STRING "The MCU used")
set(AVR_VARIANT "standard" CACHE STRING "The variant used")
set(AVR_PROGRAMMER "pi_zero_w" CACHE STRING "The programmer used")
set(AVR_CONFIG_FILE ${DRONE_ROOT}/drone_hat/avrdude_gpio.conf)
set(AVR_UPLOADTOOL_PORT "gpio")
set(AVR_MCU_SPEED 8000000UL)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/avr-gcc-toolchain.cmake")

project(DRONE_HAT C CXX ASM)
set (CMAKE_CXX_STANDARD 11)

add_subdirectory(libarduino)

add_subdirectory(contrib)

add_subdirectory(drone_hat)