#pragma once

#include "../sys/timer.h"

class Motors
{
public:
	Motors();

	void update(int throttle, int roll, int pitch, int yaw);

private:
        void setupESCs();
	void Roll(int value);
	void Pitch(int value);
	void Yaw(int value);
	void Throttle(int value);

	static constexpr int MIN_THROTTLE = 1000;
	static constexpr int MAX_THROTTLE = 2000;

private:
	Timer *m_timer;
	int m_FLspeed, m_FRspeed, m_BRspeed, m_BLspeed;
};
