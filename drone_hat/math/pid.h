#pragma once

class PID
{
public:
	PID(float Kp = 1.0, float Ki = 0.0, float Kd = 0.0);
	inline void setValues(float Kp, float Ki = 0.0, float Kd = 0.0) { m_Kp = Kp; m_Ki = Ki; m_Kd = Kd; }
	int update(int value);

	inline void setTarget(int target) { m_target = target; }

private:
	float m_value;
	float m_Kp, m_Ki, m_Kd;
	int m_last_error;
	int m_integral, m_derivative;
	int m_target;
};
