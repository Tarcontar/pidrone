set(LC_SRC 
  bosch/bme280/bme280.c
  bosch/bmi160/bmi160.c
#        NemaTode/src/GPSFix.cpp 
#        NemaTode/src/GPSService.cpp 
#        NemaTode/src/NMEACommand.cpp 
#        NemaTode/src/NMEAParser.cpp 
#        NemaTode/src/NumberConversion.cpp)
)

set(OPENCM_DIR ${BEEHIVE_ROOT}/contrib/libopencm3)
set(OPENCM_LIB ${OPENCM_DIR}/lib/libopencm3_stm32l4.a)

add_custom_command(OUTPUT ${OPENCM_LIB}
  COMMAND make
  WORKING_DIRECTORY ${OPENCM_DIR}
  COMMENT "Build opencm3 lib"
)
        
add_library(contrib ${LC_SRC} ${OPENCM_LIB})

target_include_directories(contrib PUBLIC 
  ${OPENCM_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/bosch/bme280
  ${CMAKE_CURRENT_SOURCE_DIR}/bosch/bmi160
  ${CMAKE_CURRENT_SOURCE_DIR}/NemaTode/include)

target_link_libraries(contrib ${OPENCM_LIB})

target_compile_definitions(contrib PUBLIC -DSTM32L4)
