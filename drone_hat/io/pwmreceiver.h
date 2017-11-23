#pragma once
#include <stdint.h>

#define MID 1500

class PWMReceiver
{
public:
	enum CHANNEL 
	{
			THRUST = 0,
			ROLL,
			PITCH,
			YAW,
			MODE,
			UNUSED
	};

	PWMReceiver();

  void setChannels(const int8_t channels[6]);

  uint16_t getChannel(CHANNEL c);
};
