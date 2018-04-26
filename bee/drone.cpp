#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include "hat_pcb.h"

#include <string>
#include <array>
#include "io/serial.h"

#include "actuators/motors.h"

Serial ser(115200);
Motors *motors;

void setup_status_led()
{
	rcc_periph_clock_enable(_LED_STATUS_RCC_PORT);
	gpio_clear(_LED_STATUS_PORT, _LED_STATUS_PIN);
	gpio_set_mode(_LED_STATUS_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, _LED_STATUS_PIN);
}

void sys_tick_handler(void)
{
	gpio_toggle(_LED_STATUS_PORT, _LED_STATUS_PIN);
	ser << "loop";
}

int main(void)
{
	rcc_clock_setup_in_hse_16mhz_out_72mhz();//eig sind 8mhz verbaut aber geht nur mit 16?!?!
	
	setup_status_led();

	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_set_reload(71999999);
	systick_interrupt_enable();
	systick_counter_enable();

	ser << "System starting";
	
	motors = new Motors();
	motors->setupESCs();

	while(1) {}

	return 0;
}
