#include "actuators/motors.h"
#include "io/serial.h"
#include "sys/mcu.h"
#include "sys/handler.h"
#include <stdint.h>
#include <string>

//includes for spi
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>


void spi_setup()
{
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_SPI1);

	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO4 | GPIO5 | GPIO7);
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO6);

	spi_reset(SPI1);
	spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_64, SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
			SPI_CR1_CPHA_CLK_TRANSITION_2, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

	//neede even if we handle the slave selects ourselves
	spi_enable_software_slave_management(SPI1);
	spi_set_nss_high(SPI1);
	
	spi_enable(SPI1);
}

int main(void)
{
	MCU::setup();
	
	spi_setup();

	Serial ser(9600);
	
	ser << "Starting MCU..." << ser.endl;

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
