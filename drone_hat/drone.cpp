#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <string>
#include <array>

uint32_t temp32;
std::array<uint32_t, 4> test;

static void gpio_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_set(GPIOC, GPIO8);
	gpio_clear(GPIOC, GPIO9);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO8 | GPIO9);
}

void sys_tick_handler(void)
{
	temp32++;
	if (temp32 == 1000)
	{
		gpio_toggle(GPIOC, GPIO8 | GPIO9);
		temp32 = 0;
	}
}

int main(void)
{
	rcc_clock_setup_in_hse_16mhz_out_72mhz();//eig sind 8mhz verbaut aber geht nur mit 16?!?!
	gpio_setup();

	temp32 = 0;

	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_set_reload(499);
	systick_interrupt_enable();
	systick_counter_enable();


	/* Blink the LEDs (PC8 and PC9) on the board. */
	while(1) {}

	return 0;
}
