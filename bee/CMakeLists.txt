set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")

add_executable(bee 
	drone.cpp
	io/serial.cpp
	io/usart.cpp
	sys/timer.cpp
	sys/mcu.cpp 
	sys/handler.cpp
	sys/status.cpp
	actuators/motors.cpp)

target_link_libraries(bee contrib)

find_Program(OPENOCD NAMES openocd)

add_custom_target(upload
	DEPENDS bee
	COMMAND ${OPENOCD}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

find_package(PythonInterp REQUIRED)

add_custom_target(listen
	DEPENDS upload
	COMMAND ${PYTHON_EXECUTABLE} ./listen.py
	WORKING_DIRECTORY ${BEEHIVE_ROOT}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/openocd.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
