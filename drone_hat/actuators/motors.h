#pragma once
#include <Servo.h>

#define MIN_THROTTLE 1000
#define MAX_THROTTLE 1700

class Motors
{
public:

	Motors(int FLpin, int FRpin, int BRpin, int BLpin);
	~Motors();
	
	void SetupESCs();
	
	void Update(int throttle, int roll, int pitch, int yaw);
	
private:
	void Roll(int value);
	void Pitch(int value);
	void Yaw(int value);
	void Throttle(int value);

private:
	Servo m_FLmotor, m_FRmotor, m_BRmotor, m_BLmotor;	
	int m_FLspeed, m_FRspeed, m_BRspeed, m_BLspeed;

};