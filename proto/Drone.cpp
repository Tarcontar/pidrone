#include "Drone.h"

Drone::Drone(int motorFL, int motorFR, int motorBL, int motorBR)
{
	m_motorFL.attach(motorFL);
	m_motorFR.attach(motorFR);
	m_motorBL.attach(motorBL);
	m_motorBR.attach(motorBR);
}

void Drone::Hover()
{
	//get sensor data

	setMotorSpeed();
}

void Drone::Foreward(float value)
{
	Pitch(value);
	Throttle(value);
}

void Drone::Back(float value)
{
	Pitch(-value);
	Throttle(value);
}

void Drone::Left(float value)
{
	Roll(-value);
	Throttle(value);
}

void Drone::Right(float value)
{
	Roll(value);
	Throttle(value);
}

void Drone::Roll(float value)
{
	value *= RESPONSIVENESS;

	m_FLSpeed += value;
	m_BLSpeed += value;
	m_FRSpeed -= value;
	m_BRSpeed -= value;

	if (m_FLSpeed > 180)
		m_FRSpeed -= m_FLSpeed - 180;
	if (m_BLSpeed > 180)
		m_BRSpeed -= m_BLSpeed - 180;
	if (m_FRSpeed > 180)
		m_FLSpeed -= m_FRSpeed - 180;
	if (m_BRSpeed > 180)
		m_BLSpeed -= m_BRSpeed - 180;

	setMotorSpeed();
}

void Drone::Pitch(float value)
{
	value *= RESPONSIVENESS;

	m_FLSpeed += value;
	m_BLSpeed -= value;
	m_FRSpeed += value;
	m_BRSpeed -= value;

	if (m_FLSpeed > 180)
		m_BLSpeed -= m_FLSpeed - 180;
	if (m_BLSpeed > 180)
		m_FLSpeed -= m_BLSpeed - 180;
	if (m_FRSpeed > 180)
		m_BRSpeed -= m_FRSpeed - 180;
	if (m_BRSpeed > 180)
		m_FRSpeed -= m_BRSpeed - 180;

	setMotorSpeed();
}

void Drone::Yaw(float value)
{
	value *= RESPONSIVENESS;

	//depends on rotation direction of the motors!!
	m_FLSpeed += value;
	m_BLSpeed -= value;
	m_FRSpeed -= value;
	m_BRSpeed *= value;

	if (m_FLSpeed > 180)
		m_FRSpeed -= m_FLSpeed - 180;
	if (m_BLSpeed > 180)
		m_BRSpeed -= m_BLSpeed - 180;
	if (m_FRSpeed > 180)
		m_FLSpeed -= m_FRSpeed - 180;
	if (m_BRSpeed > 180)
		m_BLSpeed -= m_BRSpeed - 180;

	setMotorSpeed();
}

void Drone::Throttle(float value)
{
	value *= RESPONSIVENESS;

	m_FLSpeed += value;
	m_BLSpeed += value;
	m_FRSpeed += value;
	m_BRSpeed += value;
	setMotorSpeed();
}

void Drone::setMotorSpeed()
{
	m_motorFL.write(m_FLSpeed);
	m_motorFR.write(m_FRSpeed);
	m_motorBL.write(m_BLSpeed);
	m_motorBR.write(m_BRSpeed);
}