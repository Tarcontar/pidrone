#include "Drone.h"
#include "BMI160/BMI160Gen.h"
#include "PWMReceiver.h"


Drone::Drone(int motorFL, int motorFR, int motorBL, int motorBR, int channel1, int channel2, int channel3, int channel4)
{
	m_motorFL.attach(motorFL);
	m_motorFR.attach(motorFR);
	m_motorBL.attach(motorBL);
	m_motorBR.attach(motorBR);

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

	receiver = PWMReceiver(channel1, channel2, channel3, channel4);
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

	double dt = (double)(micros() - timer) / 1000000; //delta time
	m_timer = micros();

	double roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
	double pitch = atan2(-accX, accZ) * RAD_TO_DEG;

	//WTF?
	double gyroXrate = gyroX / 131.0; //convert to deg/s
	double gyroYrate = gyroY / 131.0;

	double kalAngleX, kalAngleY, kalAngleZ;

	//this fixes transition problem from -180 to 180
	if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90))
	{
		kalmanY.setAngle(pitch);
		kalAngleY = pitch;
		gyroYangle = pitch;
	}
	else
	{
		kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);
	}

	if (abs(kalAngleY) > 90)
	{
		gyroXrate = -gyroXrate;
	}

	kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt);

	gyroXangle += gyroXrate * dt;
	gyroYangle += gyroYrate * dt;

	//reset gyro angle when it has drifted too much
	if (gyroXangle < -180 || gyroXangle > 180)
		gyroXangle = kalAngleX;
	if (gyroYangle < -180 || gyroYangle > 180)
		gyroYangle = kalAngleY;



	//print data
	Serial.print("roll: ");
	Serial.print(kalAngleX);
	Serial.print("  ");
	Serial.print("pitch: ");
	Serial.print(kalAngleY);
	Serial.println();

  //get receiver values
  int chan1 = receiver.getChannel(1);
  int chan2 = receiver.getChannel(2);
  int chan3 = receiver.getChannel(3);
  int chan4 = receiver.getChannel(4);


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

