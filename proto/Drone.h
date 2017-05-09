#pragma once
#include <Servo.h>

#define RESPONSIVENESS 1.0

class Drone
{
public:
	Drone(int motorFL, int motorFR, int motorBL, int motorBR);

	//to be called in the loop
	void Hover();

	void Foreward(float value);
	void Back(float value);
	void Left(float value);
	void Right(float value);

	//positive value -> cw, negative value ccw
	void Roll(float value);
	//positive value -> cw, negative value ccw
	void Pitch(float value);
	//positive value -> cw, negative value ccw
	void Yaw(float value);

	void Throttle(float value);
private:
	void setMotorSpeed();

	int m_FLSpeed = 0;
	int m_FRSpeed = 0;
	int m_BLSpeed = 0;
	int m_BRSpeed = 0;

	Servo m_motorFL;
	Servo m_motorFR;
	Servo m_motorBL;
	Servo m_motorBR;
};