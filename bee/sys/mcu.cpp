#include "mcu.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

void* __dso_handle;
static void clock_setup();

void MCU::setup()
{
	clock_setup();
}

static void clock_setup()
{
	rcc_osc_on(RCC_HSI16);

	flash_prefetch_enable();
	flash_set_ws(4);
	flash_dcache_enable();
	flash_icache_enable();
	rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_HSI16, 4, 40, 0, 0, RCC_PLLCFGR_PLLR_DIV2);

	rcc_osc_on(RCC_PLL);
	//rcc_wait_for_osc_ready();
}
