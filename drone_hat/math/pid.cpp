#include "pid.h"

//implement TPA for PID

PID::PID() 
{
	m_last_error = 0;
	m_integral = 0;
}

float PID::Update(float value)
{
	float error = m_target - value;

	m_integral += error;

	m_derivative = error - m_last_error;

	m_last_error = error;

	return (m_Kp * error) + (m_Ki * m_integral) + (m_Kd * m_derivative);
}
