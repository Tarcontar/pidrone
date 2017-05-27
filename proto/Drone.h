#pragma once
#include <Servo.h>
#include "PWMReceiver.h"
#include "Kalman.h"
#include "Arduino.h"

#define RESPONSIVENESS 1.0

#define BMI160_PIN 2

class Drone
{
public:
	Drone();
  void SetPins(int motorFL, int motorFR, int motorBL, int motorBR, int channel1, int channel2, int channel3, int channel4);

	void Update();

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
	float convertRawGyro(int gRaw);
	float convertRawAccel(int aRaw);

	int m_FLSpeed = 0;
	int m_FRSpeed = 0;
	int m_BLSpeed = 0;
	int m_BRSpeed = 0;

	Servo m_motorFL;
	Servo m_motorFR;
	Servo m_motorBL;
	Servo m_motorBR;

	PWMReceiver receiver;

	Kalman m_kalmanX, m_kalmanY;

	int m_axRaw, m_ayRaw, m_azRaw;
	int m_gxRaw, m_gyRaw, m_gzRaw;

	double m_accX, m_accY, m_accZ;
	double m_gyroX, m_gyroY, m_gyroZ;

	double m_roll, m_pitch;

	long m_timer;
};
