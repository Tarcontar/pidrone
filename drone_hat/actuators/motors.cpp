#include "motors.h"
#include <Arduino.h>

Motors::Motors(int FLpin, int FRpin, int BRpin, int BLpin) :
 m_FLspeed(MIN_THROTTLE), m_FRspeed(MIN_THROTTLE), m_BRspeed(MIN_THROTTLE), m_BLspeed(MIN_THROTTLE)
{
	m_FLmotor.attach(FLpin);
	m_FRmotor.attach(FRpin);
	m_BRmotor.attach(BRpin);
	m_BLmotor.attach(BLpin);
}

void Motors::SetupESCs()
{
	int min = 1000;
	int max = 2000;
	m_motorFL.writeMicroseconds(min);
	m_motorFR.writeMicroseconds(min);
	m_motorBR.writeMicroseconds(min);
	m_motorBL.writeMicroseconds(min);
	delay(3000);
	m_motorFL.writeMicroseconds(max);
	m_motorFR.writeMicroseconds(max);
	m_motorBR.writeMicroseconds(max);
	m_motorBL.writeMicroseconds(max);
	delay(2);
	m_motorFL.writeMicroseconds(min);
	m_motorFR.writeMicroseconds(min);
	m_motorBR.writeMicroseconds(min);
	m_motorBL.writeMicroseconds(min);
}

void Update(int throttle, int roll, int pitch, int yaw)
{
	Throttle(throttle);
	Roll(roll);
	Pitch(pitch);
	YAW(yaw);
	
	m_FLspeed = min(max(m_FLspeed, MIN_THROTTLE), MAX_THROTTLE);
	m_FRspeed = min(max(m_FRspeed, MIN_THROTTLE), MAX_THROTTLE);
	m_BRspeed = min(max(m_BRspeed, MIN_THROTTLE), MAX_THROTTLE);
	m_BLspeed = min(max(m_BLspeed, MIN_THROTTLE), MAX_THROTTLE);
	
	m_motorFL.writeMicroseconds(m_FLspeed);
	m_motorFR.writeMicroseconds(m_FRspeed);
	m_motorBR.writeMicroseconds(m_BRspeed);
	m_motorBL.writeMicroseconds(m_BLspeed);
}

void Roll(int value)
{
	m_FLspeed += value;
	m_BLspeed += value;
	m_FRspeed -= value;
	m_BRspeed -= value;
}

void Pitch(int value)
{
	m_FLspeed += value;
	m_BLspeed -= value;
	m_FRspeed += value;
	m_BRspeed -= value;
}

void Yaw(int value)
{
	m_FLspeed -= value;
	m_BLspeed += value;
	m_FRspeed += value;
	m_BRspeed -= value;
}

void Throttle(int value)
{
	m_FLspeed += value;
	m_BLspeed += value;
	m_FRspeed += value;
	m_BRspeed += value;
}