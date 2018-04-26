#pragma once
#include <stdint.h>

class USART
{
public:
	void begin(int baud);
	
	void write(uint16_t data);

private:

};
