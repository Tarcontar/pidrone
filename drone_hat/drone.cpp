#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

static void gpio_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set(GPIOC, GPIO8);
	gpio_clear(GPIOC, GPIO9);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO8 | GPIO9);
}

int main(void)
{
	//rcc_clock_setup_in_hse_8mhz_out_72mhz();
	gpio_setup();


	/* Blink the LEDs (PC8 and PC9) on the board. */
	while (1)
	{
		gpio_toggle(GPIOC, GPIO8);
		gpio_toggle(GPIOC, GPIO9);

		for (int i = 0; i < 100000; i++)
			__asm__("nop");
	}

	return 0;
}
