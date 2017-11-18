#pragma once

class PWMReceiver
{
public:
	PWMReceiver();

  void SetChannels(int ch_1, int ch_2 = -1, int ch_3 = -1, int ch_4 = -1, int ch_5 = -1, int ch_6 = -1);

  long getChannel(int channel);
};
