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
	ser << "System starting" << ser.endl;

	/*
	Handler::queryTask([ser]()
	{
		ser << "This is a test" << ser.endl;
	});
	*/

	while(1)
	{
		//keep this for future testing
		uint32_t delay = 500000;
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		ser << "test1" << ser.endl;
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		ser << "test2" << ser.endl;
	}
	return 0;
}
