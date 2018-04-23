set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT}")

add_executable(drone_hat 
        drone.cpp
	io/serial.cpp
	io/pwm.cpp)
        # math/pid.cpp
		# io/sensors.cpp
		# io/pwmreceiver.cpp
		# actuators/motors.cpp)

target_link_libraries(drone_hat contrib)

#set_target_properties(drone_hat PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

find_program(OPENOCD NAMES openocd)

message("${CMAKE_CURRENT_SOURCE_DIR}/openocd.conf")
message("${CMAKE_CURRENT_BINARY_DIR}")

add_custom_target(upload
	COMMAND ${OPENOCD}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/openocd.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_dependencies(upload drone_hat)
