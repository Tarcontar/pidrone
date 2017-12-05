set(LC_SRC 
        bosch/bme280/bme280.c
        bosch/bmi160/bmi160.c
#        NemaTode/src/GPSFix.cpp 
#        NemaTode/src/GPSService.cpp 
#        NemaTode/src/NMEACommand.cpp 
#        NemaTode/src/NMEAParser.cpp 
#        NemaTode/src/NumberConversion.cpp)
)

set(OPENCM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libopencm3)
set(OPENCM_LIB ${OPENCM_DIR}/lib/libopencm3_stm32f1.a)

add_custom_target(OPENCM
        COMMAND make
        WORKING_DIRECTORY ${OPENCM_DIR}
)
        
add_library(contrib ${LC_SRC})

target_include_directories(contrib PUBLIC 
        ${OPENCM_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/bosch/bme280
        ${CMAKE_CURRENT_SOURCE_DIR}/bosch/bmi160
        ${CMAKE_CURRENT_SOURCE_DIR}/NemaTode/include)

add_dependencies(contrib OPENCM)

target_link_libraries(contrib ${OPENCM_LIB})

target_compile_definitions(contrib PUBLIC -DSTM32F1)
