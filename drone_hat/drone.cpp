#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include "hat_pcb.h"

uint32_t led_status_counter;

void setup_status_led()
{
	rcc_periph_clock_enable(_LED_STATUS_RCC_PORT);
	gpio_clear(_LED_STATUS_PORT, _LED_STATUS_PIN);
	gpio_set_mode(_LED_STATUS_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, _LED_STATUS_PIN);
	led_status_counter = 0;
}

void sys_tick_handler(void)
{
	led_status_counter++;
	if (led_status_counter == 1000)
	{
		gpio_toggle(_LED_STATUS_PORT, _LED_STATUS_PIN);
		led_status_counter = 0;
	}
}

int main(void)
{
	rcc_clock_setup_in_hse_16mhz_out_72mhz();//eig sind 8mhz verbaut aber geht nur mit 16?!?!

	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_set_reload(71999);
	systick_interrupt_enable();
	systick_counter_enable();

	while(1) {}

	return 0;
}
