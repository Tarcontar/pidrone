#include "hat_pcb.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/usart.h>

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
}

int main()
{
	setup_clock();
	USART::setup();
	
	USART::write('X');
	printf("hi guys!\n");

	//status led
	rcc_periph_clock_enable(LED_STATUS_RCC_PORT);
	gpio_mode_setup(LED_STATUS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_STATUS_PIN);
	gpio_set(LED_STATUS_PORT, LED_STATUS_PIN);

	while(1)
	{
		gpio_toggle(LED_STATUS_PORT, LED_STATUS_PIN);
		//keep this for future testing
		uint32_t delay = 8000000;
		for (uint32_t i = 0; i < delay; i++)
			__asm__("nop");
	}
	return 0;
}
