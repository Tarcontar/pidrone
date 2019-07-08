#include "hat_pcb.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

int main()
{
	//status led
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
	gpio_set(GPIOC, GPIO12);

	while(1)
	{
		gpio_toggle(GPIOC, GPIO12);
		//keep this for future testing
		uint32_t delay = 360000;
		for (uint32_t i = 0; i < delay; i++)
			__asm__("nop");
	}
	return 0;
}
