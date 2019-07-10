#include "usart.h"

#include "../hat_pcb.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

void USART::setup()
{
	rcc_periph_clock_enable(USART_RCC_PORT);
	rcc_periph_clock_enable(USART_RCC_USART_PORT);

	gpio_set_mode(USART_PORT, GPIO_MODE_OUTPUT_AF, GPIO_PUPD_NONE, USART_TX | USART_RX);

	usart_set_baudrate(USART, USART_BAUD);
	usart_set_databits(USART, 8);
	usart_set_stopbits(USART, USART_STOPBITS_1);
	usart_set_mode(USART, USART_MODE_TX);
	usart_set_parity(USART, USART_PARITY_NONE);
	usart_set_flow_control(USART, USART_FLOWCONTROL_NONE);
	
	usart_enable(USART);
}

void USART::write(uint16_t data)
{
	usart_send_blocking(USART, data);
}

int _write(int file, char *ptr, int len)
{
	int i;
	
	if (file == STDOUT_FILENO || file == STDERR_FILENO)
	{
		for (i = 0; i < len; i++) write('\n');
		return i;
	}
	errno = EIO;
	return -1;
}
