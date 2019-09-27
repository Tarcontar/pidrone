#include "systick.h"
#include <libopencm3/cm3/systick.h>

volatile int32_t system_micros;

extern "C"
{
    void sys_tick_handler(void)
    {
        system_micros++;
    }
}

void SysTick::setup()
{
    system_micros = 0;
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
    systick_set_reload(10);
    systick_counter_enable();
    systick_interrupt_enable();
}

void SysTick::sleep_mills(uint32_t delay) { sleep_mics(delay * 1000); }

void SysTick::sleep_mics(uint32_t delay)
{
    int32_t current = system_micros;
    system_micros -= delay;
    while (current > system_micros);
}
