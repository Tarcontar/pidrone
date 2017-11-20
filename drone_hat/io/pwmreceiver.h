#pragma once
#include <stdint.h>

class PWMReceiver
{
public:
	PWMReceiver();

  void SetChannels(const int8_t channels[6]);

  uint16_t getChannel(int8_t channel);
};
