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
		uint32_t delay = 750000;
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		ser << "Hi from Serial!" << ser.endl;
		ser << "int test: " << 545 << ser.endl;
		ser << "float test: " << 3.14 << ser.endl;

	}
	return 0;
}
