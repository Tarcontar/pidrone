#include "actuators/motors.h"
#include "io/serial.h"
#include "sys/mcu.h"
#include "sys/handler.h"
#include <stdint.h>
#include <string>

#include "io/usart.h"

int main(void)
{
	MCU::setup();

	USART::begin(9600);

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
		USART::write('h');
		USART::write('i');
		USART::write('\r');
		USART::write('\n');
	}
	return 0;
}
