set(LC_SRC 
        bosch/bme280/bme280.c
        bosch/bmi160/bmi160.c
        NemaTode/src/GPSFix.cpp 
        NemaTode/src/GPSService.cpp 
        NemaTode/src/NMEACommand.cpp 
        NemaTode/src/NMEAParser.cpp 
        NemaTode/src/NumberConversion.cpp)

add_library(contrib ${LC_SRC})

target_include_directories(contrib PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/bosch/bme280
        ${CMAKE_CURRENT_SOURCE_DIR}/bosch/bmi160
        ${CMAKE_CURRENT_SOURCE_DIR}/NemaTode/include)