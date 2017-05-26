#pragma once

class PID
{
public:
	PID(float Kp, float Ki = 0.0, float Kd = 0.0);
	float Update(float value);

	inline void SetTarget(float target) { m_target = target; }

private:
	float m_value;
	float m_Kp, m_Ki, m_Kd;
	float m_last_error;
	float m_integral, m_derivative;
	float m_target;
};