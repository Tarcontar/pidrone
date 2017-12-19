#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include "io/serial.h"



Serial serial;
uint32_t temp32;

static void gpio_setup(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);

	gpio_set(GPIOA, GPIO8); /* LED0 off */
	gpio_clear(GPIOA, GPIO9); /* LED1 off */
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO8 | GPIO9);

}

void sys_tick_handler(void)
{
	temp32++;

	/* We call this handler every 1ms so 1000ms = 1s on/off. */
	if (temp32 == 1000) {
		gpio_toggle(GPIOC, GPIO8); /* LED2 on/off */
		gpio_toggle(GPIOC, GPIO9); /* LED2 on/off */
		temp32 = 0;
	}
}


int main(void)
{
	rcc_clock_setup_in_hse_8mhz_out_72mhz();
	gpio_setup();

	temp32 = 0;

	/* 72MHz / 8 => 9000000 counts per second */
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

	/* 9000000/9000 = 1000 overflows per second - every 1ms one interrupt */
	/* SysTick interrupt every N clock pulses: set reload to N-1 */
	systick_set_reload(8999);

	systick_interrupt_enable();

	/* Start counting. */
	systick_counter_enable();

	while (1); /* Halt. */

	return 0;
}