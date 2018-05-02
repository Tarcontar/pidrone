#pragma once
#include <stdint.h>

class USART
{
public:
	static void write(uint16_t data);

private:
	static void setup();
	static bool m_ready;
};
