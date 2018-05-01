#include "actuators/motors.h"
#include "io/serial.h"
#include "sys/mcu.h"
#include "sys/handler.h"
#include <stdint.h>
#include <string>


int main(void)
{
	MCU::setup();

	Serial ser(9600);

	while(1)
	{
		//keep this for future testing
		uint32_t delay = 4500000;
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		float t = 2.75;
		ser << "Hi from Serial! " << 545 << " " << 3.14 << " " << t << ser.endl;

	}
	return 0;
}
