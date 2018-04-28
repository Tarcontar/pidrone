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

	Motors motors;
	motors.setupESCs();

	Handler::queryTask([ser]()
	{
		ser << "This is a test" << ser.endl;
	});

	while(1)
	{
		//keep this for future testing
		/*
		uint32_t delay = 5000000;
		//motors->update(1700);

		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");

		//motors->update(300);
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		*/
	}

	return 0;
}
