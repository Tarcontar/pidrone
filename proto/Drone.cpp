#include "Drone.h"
#include "BMI160Gen.h"
#include "PWMReceiver.h"
#include <EEPROM.h>

Drone::Drone()
{
}

void Drone::Setup()
{
	if (!BMI160.begin(BMI160GenClass::SPI_MODE, BMI160_PIN))
	{
		//error
	}

	BMI160.setGyroRate(25);
	BMI160.setAccelerometerRate(25);

	//Set the accelerometer range to 250 degrees/second
	BMI160.setGyroRange(250);
	BMI160.setAccelerometerRange(2);

	delay(200); // needed so accelerometer reads valid values


	BMI160.readAccelerometer(m_axRaw, m_ayRaw, m_azRaw);
	m_accX = convertRawAccel(m_axRaw);
	m_accY = convertRawAccel(m_ayRaw);
	m_accZ = convertRawAccel(m_azRaw);

	double roll = atan(m_accY / sqrt(m_accX * m_accX + m_accZ * m_accZ)) * RAD_TO_DEG;
	double pitch = atan2(-m_accX, m_accZ) * RAD_TO_DEG;

	m_kalmanX.setAngle(roll);
	m_kalmanY.setAngle(pitch);

	m_pid_roll.SetTarget(roll);
	m_pid_roll.SetValues(1.0);
	m_pid_pitch.SetTarget(pitch);
	m_pid_pitch.SetValues(1.0);

	m_motorFL = MIN_THROTTLE;
	m_motorFR = MIN_THROTTLE;
	m_motorBL = MIN_THROTTLE;
	m_motorBR = MIN_THROTTLE;
}

void Drone::SetPins(int motorFL, int motorFR, int motorBL, int motorBR, int channel1, int channel2, int channel3, int channel4)
{
  m_motorFL.attach(motorFL);
  m_motorFR.attach(motorFR);
  m_motorBL.attach(motorBL);
  m_motorBR.attach(motorBR);

  //setup ESCS
  m_motorFL.writeMicroseconds(1000);
  m_motorFR.writeMicroseconds(1000);
  m_motorBL.writeMicroseconds(1000);
  m_motorBR.writeMicroseconds(1000);
  delay(3000);
  m_motorFL.writeMicroseconds(2000);
  m_motorFR.writeMicroseconds(2000);
  m_motorBL.writeMicroseconds(2000);
  m_motorBR.writeMicroseconds(2000);
  delay(200);

  receiver.SetChannels(channel1, channel2, channel3, channel4);
}

void Drone::Update()
{
	//read raw gyro measurements from device
	BMI160.readGyro(m_gxRaw, m_gyRaw, m_gzRaw);
	BMI160.readAccelerometer(m_axRaw, m_ayRaw, m_azRaw);

	//convert the raw gyro data to degrees/second
	float gyroX = convertRawGyro(m_gxRaw);
	float gyroY = convertRawGyro(m_gyRaw);
	float gyroZ = convertRawGyro(m_gzRaw);

	float accX = convertRawAccel(m_axRaw);
	float accY = convertRawAccel(m_ayRaw);
	float accZ = convertRawAccel(m_azRaw);

	double dt = (double)(micros() - m_timer) / 1000000; //delta time
	m_timer = micros();

	double roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
	double pitch = atan2(-accX, accZ) * RAD_TO_DEG;

	double gyroXrate = gyroX / 131.0; //convert to deg/s
	double gyroYrate = gyroY / 131.0;

	double kalAngleX, kalAngleY;

	//this fixes transition problem from -180 to 180
	if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90))
	{
		m_kalmanY.setAngle(pitch);
		kalAngleY = pitch;
	}
	else
	{
		kalAngleY = m_kalmanY.getAngle(pitch, gyroYrate, dt);
	}

	if (abs(kalAngleY) > 90)
	{
		gyroXrate = -gyroXrate;
	}

	kalAngleX = m_kalmanX.getAngle(roll, gyroXrate, dt);

	//get receiver values
	int chan1 = receiver.getChannel(1);
	int chan2 = receiver.getChannel(2);
	int chan3 = receiver.getChannel(3);
	int chan4 = receiver.getChannel(4);

	//apply throttle channel here
	m_FLSpeed = MIN_THROTTLE;
	m_FRSpeed = MIN_THROTTLE;
	m_BLSpeed = MIN_THROTTLE;
	m_BRSpeed = MIN_THROTTLE;

	Roll(m_pid_roll.Update(kalAngleX));
	Pitch(m_pid_pitch.Update(kalAngleY));

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

	if (m_FLSpeed > MAX_THROTTLE)
		m_FRSpeed -= m_FLSpeed - MAX_THROTTLE;
	if (m_BLSpeed > MAX_THROTTLE)
		m_BRSpeed -= m_BLSpeed - MAX_THROTTLE;
	if (m_FRSpeed > MAX_THROTTLE)
		m_FLSpeed -= m_FRSpeed - MAX_THROTTLE;
	if (m_BRSpeed > MAX_THROTTLE)
		m_BLSpeed -= m_BRSpeed - MAX_THROTTLE;

	setMotorSpeed();
}

void Drone::Pitch(float value)
{
	value *= RESPONSIVENESS;

	m_FLSpeed += value;
	m_BLSpeed -= value;
	m_FRSpeed += value;
	m_BRSpeed -= value;

	if (m_FLSpeed > MAX_THROTTLE)
		m_BLSpeed -= m_FLSpeed - MAX_THROTTLE;
	if (m_BLSpeed > MAX_THROTTLE)
		m_FLSpeed -= m_BLSpeed - MAX_THROTTLE;
	if (m_FRSpeed > MAX_THROTTLE)
		m_BRSpeed -= m_FRSpeed - MAX_THROTTLE;
	if (m_BRSpeed > MAX_THROTTLE)
		m_FRSpeed -= m_BRSpeed - MAX_THROTTLE;

	setMotorSpeed();
}

void Drone::Yaw(float value)
{
	value *= RESPONSIVENESS;

	//depends on rotation direction of the motors!!
	m_FLSpeed += value;
	m_BLSpeed -= value;
	m_FRSpeed -= value;
	m_BRSpeed += value;

	if (m_FLSpeed > MAX_THROTTLE)
		m_FRSpeed -= m_FLSpeed - MAX_THROTTLE;
	if (m_BLSpeed > MAX_THROTTLE)
		m_BRSpeed -= m_BLSpeed - MAX_THROTTLE;
	if (m_FRSpeed > MAX_THROTTLE)
		m_FLSpeed -= m_FRSpeed - MAX_THROTTLE;
	if (m_BRSpeed > MAX_THROTTLE)
		m_BLSpeed -= m_BRSpeed - MAX_THROTTLE;

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
	m_FLSpeed = max(m_FLSpeed, MIN_THROTTLE);
	m_FRSpeed = max(m_FRSpeed, MIN_THROTTLE);
	m_BLSpeed = max(m_BLSpeed, MIN_THROTTLE);
	m_BRSpeed = max(m_BRSpeed, MIN_THROTTLE);

	m_motorFL.writeMicroseconds(min(m_FLSpeed, MAX_THROTTLE));
	m_motorFR.writeMicroseconds(min(m_FRSpeed, MAX_THROTTLE));
	m_motorBL.writeMicroseconds(min(m_BLSpeed, MAX_THROTTLE));
	m_motorBR.writeMicroseconds(min(m_BRSpeed, MAX_THROTTLE));
}

float Drone::convertRawGyro(int gRaw)
{
	//since we are using 250 degrees/second range
	float g = (gRaw * 250.0) / 32768.0;
	return g;
}

float Drone::convertRawAccel(int aRaw)
{
	// since we are using 2G range
	// -2g maps to a raw value of -32768
	// +2g maps to a raw value of 32767

	float a = (aRaw * 2.0) / 32768.0;
	return a;
}

void Drone::Calibrate()
{

}

template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
	const byte* p = (const byte*)(const void*)&value;
	int i;
	for (i = 0; i < sizeof(value); i++)
		EEPROM.write(ee++, *p++);
	return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
	byte* p = (byte*)(void*)&value;
	int i;
	for (i = 0; i < sizeof(value); i++)
		*p++ = EEPROM.read(ee++);
	return i;
}

