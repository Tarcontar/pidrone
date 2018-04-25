#include "usart.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

void USART::begin(int baud)
{
	rcc_clock_setup_in_hse_8mhz_out_24mhz();
	
	rcc_periph_clock_enable(RCC_GPIOA); //needed?
	rcc_periph_clock_enable(RCC_USART1);
	
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
	
	//usart_set_baudrate(USART1, baud);
	USART_BRR(USART1) = (uint16_t) ((24000000 << 4) / (baud * 16));
	
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	
	usart_enable(USART1);
}

void USART::print(int c)
{
	usart_send_blocking(USART1, c);
}
