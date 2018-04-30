#include "actuators/motors.h"
#include "io/serial.h"
#include "sys/mcu.h"
#include "sys/handler.h"
#include <stdint.h>
#include <string>
//#include <iostream>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>

int main(void)
{
	MCU::setup();

	Serial ser(9600);

	while(1)
	{
		//keep this for future testing
		uint32_t delay = 500000;
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		ser << "Hi from Serial! "<< 3 << ser.endl;
		//std::cout << "Test this please!" << std::endl;
	}
	return 0;
}
