#pragma once
#include <stdint.h>

class USART
{
public:
	static void begin(int baud);
	
	static void write(uint16_t data);

private:
	static bool m_ready;
};
