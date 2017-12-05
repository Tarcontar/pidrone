#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

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

static void usart_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_24mhz();
	
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_USART1);
	
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
	
	//usart_set_baudrate(USART1, 38400);
	USART_BRR(USART1) = (uint16_t)((24000000 << 4) / (38400 * 16));
	
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_mode(USART1, USART_MODE_TX);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	
	usart_enable(USART1);
}

int main(void)
{
	int i, j = 0, c = 0;
	gpio_setup();
	usart_setup();
	
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
