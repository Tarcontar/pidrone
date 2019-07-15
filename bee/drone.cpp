#include "hat_pcb.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/usart.h>
#include "io/usart.h"

static void setup_clock(void)
{
	rcc_osc_on(RCC_HSE);

	flash_prefetch_enable();
	flash_set_ws(4);
	flash_dcache_enable();
	flash_icache_enable();

	// 8mhz / 2 => 4 * 40 => 160mhz / 2 => 80 mhz
	rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_HSE, 2, 40, 0, 0, RCC_PLLCFGR_PLLR_DIV2);
	rcc_osc_on(RCC_PLL);
	rcc_wait_for_osc_ready(RCC_PLL);

	rcc_set_sysclk_source(RCC_CFGR_SW_PLL);
	rcc_wait_for_sysclk_status(RCC_PLL);

	rcc_ahb_frequency = 8000000;
	rcc_apb1_frequency = 8000000;
	rcc_apb2_frequency = 8000000;
}

static void setupUart(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_UART4);

	gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO11);
	gpio_set_af(GPIOC, GPIO_AF8, GPIO10);

	usart_set_baudrate(UART4, 9600);
	usart_set_databits(UART4, 8);
	usart_set_stopbits(UART4, USART_STOPBITS_1);
	usart_set_mode(UART4, USART_MODE_TX);
	usart_set_parity(UART4, USART_PARITY_NONE);
	usart_set_flow_control(UART4, USART_FLOWCONTROL_NONE);

	usart_enable(UART4);
}

int main()
{
	setup_clock();
	setupUart();

	usart_send_blocking(UART4, 1);
	//printf("hi guys!\n");

	//status led
	rcc_periph_clock_enable(LED_STATUS_RCC_PORT);
	gpio_mode_setup(LED_STATUS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_STATUS_PIN);
	gpio_set(LED_STATUS_PORT, LED_STATUS_PIN);

	while(1)
	{
		usart_send_blocking(UART4, 2);
		gpio_toggle(LED_STATUS_PORT, LED_STATUS_PIN);
		//keep this for future testing
		uint32_t delay = 4000000;
		for (uint32_t i = 0; i < delay; i++)
			__asm__("nop");
	}
	return 0;
}
