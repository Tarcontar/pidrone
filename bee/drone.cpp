#include "actuators/motors.h"
#include "io/serial.h"
#include "sys/mcu.h"
#include "sys/handler.h"
#include <stdint.h>
#include <string>

int main(void)
{
	MCU::setup();

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
	
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(GPIOC);
	
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
	
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	
	//usart_set_baudrate(USART1, 9600);
	USART_BRR (USART1) = (uint16_t) ((24000000 << 4) / (baud * 16));
	
	while(1)
	{
		//keep this for future testing
		uint32_t delay = 5000000;
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		usart_send_blocking(USART1, 0xff);
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		usart_send_blocking(USART1, 0xff);
		
	}

	return 0;
}
