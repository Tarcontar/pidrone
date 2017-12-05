#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static void gpio_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO8 | GPIO9);

}

int main(void)
{
	int i;
	gpio_setup();
	gpio_set(GPIOC, GPIO8);
	gpio_clear(GPIOC, GPIO9);

	while(1)
	{
		gpio_toggle(GPIOC, GPIO8);
		gpio_toggle(GPIOC, GPIO9);
		for(i = 0; i < 1000000; i++)
		{
			__asm__("nop");
		}
	}
	return 0;
}
