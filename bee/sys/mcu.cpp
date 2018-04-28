#include "mcu.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

void* __dso_handle;

void MCU::setup()
{
  rcc_clock_setup_in_hse_8mhz_out_24mhz();
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  systick_set_reload(23999999UL * 2); //still not sure why we need times 2 here
  systick_interrupt_enable();
  systick_counter_enable();
}