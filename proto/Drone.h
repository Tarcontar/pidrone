#pragma once
#include <Servo.h>
#include "PWMReceiver.h"
#include "Kalman.h"
#include "Arduino.h"
#include "PID.h"
#include "HardwareSerial.h"

#define RESPONSIVENESS 0.5

#define ROLL_KP 0.9
#define ROLL_KI 0.0
#define ROLL_KD 0.0

#define PITCH_KP 0.9
#define PITCH_KI 0.0
#define PITCH_KD 0.0

#define BMI160_PIN 6
#define BME280_PIN 7
#define GPS_PIN 8

#define MIN_THROTTLE 1000
#define MAX_THROTTLE 1700



class Drone
{
public:
	Drone();

	void Setup();
	void Calibrate();

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

	inline void SetSerial(HardwareSerial &serial) { m_serial = &serial; }

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

	PID m_pid_roll;
	PID m_pid_pitch;
	PID m_pid_yaw;

	PID m_pid_height;
	PID m_pid_posX;
	PID m_pid_posY;

	int m_axRaw, m_ayRaw, m_azRaw;
	int m_gxRaw, m_gyRaw, m_gzRaw;

	double m_accX, m_accY, m_accZ;
	double m_gyroX, m_gyroY, m_gyroZ;

	double m_roll, m_pitch;

	long m_timer;

	HardwareSerial *m_serial;
};
