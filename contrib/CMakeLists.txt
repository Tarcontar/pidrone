file(GLOB_RECURSE LC_SRC ${PROJECT_SOURCE_DIR} *.c *.cpp)

add_avr_library(contrib ${LC_SRC})

target_include_directories(contrib PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/bosch/bme280
        ${CMAKE_CURRENT_SOURCE_DIR}/bosch/bmi160
        ${CMAKE_CURRENT_SOURCE_DIR}/org1411)

avr_target_link_libraries(contrib arduino)