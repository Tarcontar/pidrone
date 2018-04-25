set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")

add_executable(bee 
	drone.cpp
	io/serial.cpp
	io/pwm.cpp
	io/usart.cpp)
	# math/pid.cpp
	# io/sensors.cpp
	# io/pwmreceiver.cpp
	# actuators/motors.cpp)

target_link_libraries(bee contrib)

#set_target_properties(bee PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

find_program(OPENOCD NAMES openocd)

add_custom_target(upload
	DEPENDS bee
	COMMAND ${OPENOCD}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/openocd.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})