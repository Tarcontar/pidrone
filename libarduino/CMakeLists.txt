file(GLOB_RECURSE LA_SRC ${PROJECT_SOURCE_DIR} *.cpp)

set(LA_SRC ${LA_SRC} 
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/hooks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/WInterrupts.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/wiring.c 
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/wiring_analog.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/wiring_digital.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/wiring_shift.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/wiring_pulse.c
    ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino/wiring_pulse.S)

add_avr_library(arduino ${LA_SRC})

target_include_directories(arduino PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/cores/arduino
        ${CMAKE_CURRENT_SOURCE_DIR}/libraries/SoftwareSerial/src
		${CMAKE_CURRENT_SOURCE_DIR}/libraries/BMI160
        ${CMAKE_CURRENT_SOURCE_DIR}/variants/${AVR_VARIANT})