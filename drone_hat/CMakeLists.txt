add_executable(drone_hat 
        drone.cpp)
        # math/pid.cpp
		# io/sensors.cpp
		# io/pwmreceiver.cpp
		# actuators/motors.cpp)

target_link_libraries(drone_hat contrib)

find_program(OPENOCD NAMES openocd)

message("${CMAKE_CURRENT_SOURCE_DIR}/openocd.conf")
message("${CMAKE_CURRENT_BINARY_DIR}")

add_custom_target(upload
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:drone_hat>
									 ${CMAKE_CURRENT_SOURCE_DIR}
)

add_dependencies(upload drone_hat)