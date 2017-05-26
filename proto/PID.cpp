#include "PID.h"

PID::PID(float Kp, float Ki = 0.0, float Kd = 0.0) : m_Kp(Kp), m_Ki(Ki), m_Kd(Kd)
{
	m_last_error = 0;
	m_integral = 0;
}

void PID::Update(float value)
{
	float error = m_target - value;

	m_integral += error;

	m_derivative = error - m_last_error;

	m_last_error = error;

	return (m_Kp * error) + (m_Ki + m_integral) + (m_Kd * m_derivative);
}
