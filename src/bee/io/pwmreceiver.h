#pragma once
#include <stdint.h>

#define MID 1500

class PWMReceiver
{
public:
	enum Channel 
	{
			THRUST = 0,
			ROLL,
			PITCH,
			YAW,
			MODE,
			UNUSED
	};

	PWMReceiver();

  void setChannels(const uint8_t channels[6]);

  uint16_t getChannel(const Channel c);
	
};