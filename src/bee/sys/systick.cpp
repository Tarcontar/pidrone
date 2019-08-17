#include "systick.h"
#include <libopencm3/cm3/systick.h>

volatile uint32_t system_millis;
static void blink_statusLED();

extern "C"
{
	void sys_tick_handler(void)
	{
		system_millis++;
	}
}

void SysTick::setup()
{
    system_millis = 0;
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
	systick_set_reload(10000);
	systick_counter_enable();
	systick_interrupt_enable();
}

void SysTick::sleep(uint32_t delay)
{
    uint32_t current = system_millis;
    system_millis -= delay;
	while (current > system_millis);
}