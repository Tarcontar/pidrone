#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include <string>

#include "hat_pcb.h"
#include "io/serial.h"
#include "actuators/motors.h"

//Serial ser(9600);
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
}

int main(void)
{
	rcc_clock_setup_in_hse_8mhz_out_24mhz();

	setup_status_led();
	Serial ser(16000);
	ser << "System starting" << ser.endl;

	motors = new Motors();
	//motors->setupESCs();

	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_set_reload(23999999UL * 2); //still not sure why we need times 2 here
	systick_interrupt_enable();
	systick_counter_enable();

	while(1)
	{
		//keep this for future testing
		/*
		uint32_t delay = 5000000;
		//motors->update(1700);
		
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		
		//motors->update(300);
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		*/
	}

	return 0;
}