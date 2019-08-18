set(LC_SRC 
  bosch/bme280/bme280.c
  bosch/bmi160/bmi160.c
  bosch/bmp388/bmp3.c
  bosch/bmi088/bmi088.c
  bosch/bmi088/bmi08a.c
  bosch/bmi088/bmi08g.c
#        NemaTode/src/GPSFix.cpp 
#        NemaTode/src/GPSService.cpp 
#        NemaTode/src/NMEACommand.cpp 
#        NemaTode/src/NMEAParser.cpp 
#        NemaTode/src/NumberConversion.cpp)
)

set(OPENCM_DIR ${BEEHIVE_ROOT}/contrib/libopencm3)
set(OPENCM_LIB ${OPENCM_DIR}/lib/libopencm3_stm32l4.a)

# BM280 floating point api
add_compile_definitions(BME280_FLOAT_ENABLE)
add_compile_definitions(BMP3_DOUBLE_PRECISION_COMPENSATION)
add_compile_definitions(BMI08X_ENABLE_BMI088=1)
add_compile_definitions(BMI08X_ENABLE_BMI085=0)

add_custom_command(OUTPUT ${OPENCM_LIB}
  COMMAND make TARGETS=stm32/l4
  WORKING_DIRECTORY ${OPENCM_DIR}
  COMMENT "Build opencm3 lib"
)
        
add_library(contrib ${LC_SRC} ${OPENCM_LIB})

target_compile_definitions(contrib PUBLIC 
      BME280_FLOAT_ENABLE 
      BMP3_DOUBLE_PRECISION_COMPENSATION 
      BMI08X_ENABLE_BMI088=1 
      BMI08X_ENABLE_BMI085=0)

target_include_directories(contrib PUBLIC 
  ${OPENCM_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/bosch/bme280
  ${CMAKE_CURRENT_SOURCE_DIR}/bosch/bmi088
  ${CMAKE_CURRENT_SOURCE_DIR}/bosch/bmp388
  ${CMAKE_CURRENT_SOURCE_DIR}/NemaTode/include)

target_link_libraries(contrib ${OPENCM_LIB})

target_compile_definitions(contrib PUBLIC -DSTM32L4)
