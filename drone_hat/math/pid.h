#pragma once

class PID
{
public:
	PID(float Kp = 1.0, float Ki = 0.0, float Kd = 0.0);
	inline void setValues(float Kp, float Ki = 0.0, float Kd = 0.0) { m_Kp = Kp; m_Ki = Ki; m_Kd = Kd; }
	float update(float value);

	inline void setTarget(float target) { m_target = target; }

private:
	float m_value;
	float m_Kp, m_Ki, m_Kd;
	float m_last_error;
	float m_integral, m_derivative;
	float m_target;
};
