#include "pid.h"

//implement TPA for PID

PID::PID(float Kp, float Ki, float Kd) : m_Kp(Kp), m_Ki(Ki), m_Kd(Kd) 
{
	m_last_error = 0;
	m_integral = 0;
}

float PID::update(float value)
{
	float error = m_target - value;

	m_integral += error;

	m_derivative = error - m_last_error;

	m_last_error = error;

	return (m_Kp * error) + (m_Ki * m_integral) + (m_Kd * m_derivative);
}
