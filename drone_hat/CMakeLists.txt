add_avr_executable(drone_hat 
        drone.cpp
        math/pid.cpp
        io/sensors.cpp
        io/pwmreceiver.cpp
		actuators/motors.cpp)

avr_target_link_libraries(drone_hat arduino contrib)