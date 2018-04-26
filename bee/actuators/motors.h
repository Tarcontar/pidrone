#pragma once

#include "../io/timer.h"

#define MIN_THROTTLE 1000
#define MAX_THROTTLE 2000

class Motors
{
public:
	Motors();

	void setupESCs();
	
	void update(int throttle, int roll, int pitch, int yaw);
	
private:
	void Roll(int value);
	void Pitch(int value);
	void Yaw(int value);
	void Throttle(int value);

private:
	Timer *m_timer;
	int m_FLspeed, m_FRspeed, m_BRspeed, m_BLspeed;

};