#include "clock.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>

void Clock::setup()
{
    rcc_osc_on(RCC_HSE);

 	flash_prefetch_enable();
 	flash_set_ws(4);
 	flash_dcache_enable();
 	flash_icache_enable();

 	// 8mhz / 1 => 8 * 20 => 160mhz / 2 => 80 mhz
 	rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_HSE, 1, 20, RCC_PLLCFGR_PLLP_DIV7, RCC_PLLCFGR_PLLQ_DIV2, RCC_PLLCFGR_PLLR_DIV2);
 	rcc_osc_on(RCC_PLL);
 	rcc_wait_for_osc_ready(RCC_PLL);

 	rcc_set_sysclk_source(RCC_CFGR_SW_PLL);
 	rcc_wait_for_sysclk_status(RCC_PLL);

 	rcc_ahb_frequency = 80e6;
 	rcc_apb1_frequency = 80e6;
 	rcc_apb2_frequency = 80e6;
}
