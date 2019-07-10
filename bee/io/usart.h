#pragma once
#include <stdint.h>

class USART
{
public:
	static void setup();
	static void write(uint16_t data);
};
