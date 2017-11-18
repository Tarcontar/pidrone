add_avr_executable(drone_hat drone.cpp
io/pwmreceiver.cpp math/pid.cpp)

avr_target_link_libraries(drone_hat arduino contrib)