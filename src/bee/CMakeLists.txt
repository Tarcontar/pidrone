add_executable(bee.elf
	drone.cpp
	io/serial.cpp
	io/usart.cpp
	io/sensors.cpp
	sys/timer.cpp
	sys/clock.cpp
	sys/status.cpp
	actuators/motors.cpp
	sys/interrupt.cpp
)

target_link_libraries(bee.elf contrib)

find_Program(OPENOCD NAMES openocd)

add_custom_target(upload
	DEPENDS bee.elf
	COMMAND ${OPENOCD}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(upload_verbose
	DEPENDS bee.elf
	COMMAND ${OPENOCD} -d3
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

find_package(PythonInterp REQUIRED)

add_custom_target(listen
	DEPENDS upload
	COMMAND ${PYTHON_EXECUTABLE} ./listen.py
	WORKING_DIRECTORY ${BEEHIVE_ROOT}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/openocd.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
