add_executable(drone_hat 
        drone.cpp)
        # math/pid.cpp
		# io/sensors.cpp
		# io/pwmreceiver.cpp
		# actuators/motors.cpp)

target_link_libraries(drone_hat contrib)