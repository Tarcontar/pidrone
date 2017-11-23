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

void Motors::setupESCs()
{
	int min = 1000;
	int max = 2000;
	m_FLmotor.writeMicroseconds(min);
	m_FRmotor.writeMicroseconds(min);
	m_BRmotor.writeMicroseconds(min);
	m_BLmotor.writeMicroseconds(min);
	delay(3000);
	m_FLmotor.writeMicroseconds(max);
	m_FRmotor.writeMicroseconds(max);
	m_BRmotor.writeMicroseconds(max);
	m_BLmotor.writeMicroseconds(max);
	delay(2);
	m_FLmotor.writeMicroseconds(min);
	m_FRmotor.writeMicroseconds(min);
	m_BRmotor.writeMicroseconds(min);
	m_BLmotor.writeMicroseconds(min);
}

void Motors::update(int throttle, int roll, int pitch, int yaw)
{
	Throttle(throttle + MIN_THROTTLE);
	Roll(roll);
	Pitch(pitch);
	Yaw(yaw);
	
	m_FLspeed = min(max(m_FLspeed, MIN_THROTTLE), MAX_THROTTLE);
	m_FRspeed = min(max(m_FRspeed, MIN_THROTTLE), MAX_THROTTLE);
	m_BRspeed = min(max(m_BRspeed, MIN_THROTTLE), MAX_THROTTLE);
	m_BLspeed = min(max(m_BLspeed, MIN_THROTTLE), MAX_THROTTLE);
	
	m_FLmotor.writeMicroseconds(m_FLspeed);
	m_FRmotor.writeMicroseconds(m_FRspeed);
	m_BRmotor.writeMicroseconds(m_BRspeed);
	m_BLmotor.writeMicroseconds(m_BLspeed);
}

void Motors::Roll(int value)
{
	m_FLspeed += value;
	m_BLspeed += value;
	m_FRspeed -= value;
	m_BRspeed -= value;
}

void Motors::Pitch(int value)
{
	m_FLspeed += value;
	m_BLspeed -= value;
	m_FRspeed += value;
	m_BRspeed -= value;
}

void Motors::Yaw(int value)
{
	m_FLspeed -= value;
	m_BLspeed += value;
	m_FRspeed += value;
	m_BRspeed -= value;
}

void Motors::Throttle(int value)
{
	m_FLspeed += value;
	m_BLspeed += value;
	m_FRspeed += value;
	m_BRspeed += value;
}