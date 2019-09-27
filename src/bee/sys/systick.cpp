#include "systick.h"
#include <libopencm3/cm3/systick.h>

volatile int32_t system_100_nanos;

extern "C"
{
	void sys_tick_handler(void)
	{
		system_100_nanos++;
	}
}

void SysTick::setup()
{
    system_millis = 0;
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
	systick_set_reload(1);
	systick_counter_enable();
	systick_interrupt_enable();
}

void SysTick::sleep_mills(uint32_t delay) => sleep_100_ns(delay * 10000);
void SysTick::sleep_mics(uint32_t delay) => sleep_100_ns(delay * 10);

void SysTick::sleep_100_ns(uint32_t delay)
{
    int32_t current = system_100_nanos;
    system_100_nanos -= delay;
	while (current > system_100_nanos);
}
