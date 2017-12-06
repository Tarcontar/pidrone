#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "io/serial.h"

Serial serial;

static void delay(uint32_t nops)
{
	for (uint32_t i = 0; i < nops; i++)
		__asm__("NOP");
}

static void gpio_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO8 | GPIO9);
}

int main(void)
{
	int i, j = 0, c = 0;
	serial.begin(38400);
	gpio_setup();
	
	gpio_set(GPIOC, GPIO8);
	gpio_clear(GPIOC, GPIO9);

	while(1)
	{
		gpio_toggle(GPIOC, GPIO8);
		gpio_toggle(GPIOC, GPIO9);
		usart_send_blocking(USART1, c + '0');
		c = (c == 9) ? 0 : c + 1;
		if ((j++ % 80) == 0)
		{
			usart_send_blocking(USART1, '\r');
			usart_send_blocking(USART1, '\n');
		}
		
		delay(8000);
	}
	return 0;
}
