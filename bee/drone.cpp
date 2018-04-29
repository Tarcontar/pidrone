#include "actuators/motors.h"
#include "io/serial.h"
#include "sys/mcu.h"
#include "sys/handler.h"
#include <stdint.h>
#include <string>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

int main(void)
{
	//MCU::setup();

	/*
	Serial ser(9600);
	ser << "System starting" << ser.endl;

	Motors motors;
	motors.setupESCs();

	Handler::queryTask([ser]()
	{
		ser << "This is a test" << ser.endl;
	});
	*/

	rcc_clock_setup_in_hse_8mhz_out_24mhz();

	rcc_periph_clock_enable(RCC_GPIOA);

	rcc_periph_clock_enable(RCC_USART1);

	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);

	usart_set_baudrate(USART1, 9600);

	usart_enable(USART1);

	while(1)
	{
		//keep this for future testing
		uint32_t delay = 500000;
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		usart_send_blocking(USART1, '0');
		usart_send_blocking(USART1, '1');
		usart_send_blocking(USART1, '\r');
		usart_send_blocking(USART1, '\n');
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		usart_send_blocking(USART1, '2');
		usart_send_blocking(USART1, '3');
		usart_send_blocking(USART1, '\r');
		usart_send_blocking(USART1, '\n');
	}
	return 0;
}
