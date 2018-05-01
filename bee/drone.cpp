#include "actuators/motors.h"
#include "io/serial.h"
#include "io/sensors.h"
#include "sys/mcu.h"
#include "sys/handler.h"
#include <stdint.h>
#include <string>

//for testing only
#include <libopencm3/stm32/spi.h>

int main(void)
{
	MCU::setup();
	Serial ser(9600);
	ser << "Starting MCU..." << ser.endl;
	
	Sensors sensors;
	sensors.setup();

	while(1)
	{
		//keep this for future testing
		uint32_t delay = 4500000;
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		float t = 2.75;
		ser << "Hi from Serial! " << 545 << " " << 3.14 << " " << t << ser.endl;
		
		uint8_t testByte = 67;
		spi_send(SPI1, testByte);
		ser << "SPI_send: " << testByte << ser.endl;
		
		testByte = spi_read(SPI1);

		ser << "SPI_rec: " << testByte << ser.endl;
	}
	return 0;
}
