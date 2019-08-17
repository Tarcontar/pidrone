#include "hat_pcb.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/systick.h>
#include "io/usart.h"

uint32_t system_millis;
static void blink_statusLED();

extern "C"
{
	void sys_tick_handler(void)
	{
		system_millis++;
	}
}

static void msleep(uint32_t delay)
{
	uint32_t wake = system_millis + delay;
	while (wake > system_millis);
}

static void setup_systick()
{
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
	systick_set_reload(10000);
	systick_counter_enable();
	systick_interrupt_enable();
}

static void setup_clock()
{
 	rcc_osc_on(RCC_HSE);

 	flash_prefetch_enable();
 	flash_set_ws(4);
 	flash_dcache_enable();
 	flash_icache_enable();

 	// 8mhz / 2 => 4 * 40 => 160mhz / 2 => 80 mhz
 	rcc_set_main_pll(RCC_PLLCFGR_PLLSRC_HSE, 2, 40, 0, 0, RCC_PLLCFGR_PLLR_DIV2);
 	rcc_osc_on(RCC_PLL);
 	rcc_wait_for_osc_ready(RCC_PLL);

 	rcc_set_sysclk_source(RCC_CFGR_SW_PLL);
 	rcc_wait_for_sysclk_status(RCC_PLL);

 	rcc_ahb_frequency = 80e6;
 	rcc_apb1_frequency = 80e6;
 	rcc_apb2_frequency = 80e6;
}


static void write_uart(int data)
{
 	usart_send_blocking(UART4_BASE, data);
}

// //does this configure the stm32 as a spi master? do we need it as slave?
// static void setup_spi()
// {
// 	rcc_periph_clock_enable(RCC_GPIOA | RCC_SPI1);
// 	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, GPIO5 | GPIO6 | GPIO7);
// 	gpio_set_af(GPIOA, GPIO_AF6, GPIO5 | GPIO6 | GPIO7);
// 	gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, GPIO5 | GPIO7);
// 	gpio_set(GPIOA, GPIO4);
// 	gpio_mode_setup(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO4);

// 	cr_tmp = SPI_CR1_BAUDRATE_FPCLK_DIV_8 | SPI_CR1_MSTR | SPI_CR1_SPE | SPI_CR1_CPHA | SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE;

// 	SPI_CR2(SPI1) |= SPI_CR2_SSOE;
// 	SPI_CR1(SPI1) = cr_tmp;
// }

// uint16_t read_spi_reg(int reg)
// {
// 	uint16_t d1, d2;

// 	d1 = 0x80 | (reg & 0x3f);
// 	gpio_clear(GPIOA, GPIO4);
// 	spi_send(SPI1, d1);
// 	d2 = spi_read(SPI1);
// 	d2 <<= 8;

// 	spi_send(SPI1, 0);
// 	d2 |= spi_read(SPI1);
// 	gpio_set(GPIOA, GPIO4);
// 	return d2;
// }

// void write_spi_reg(uint8_t reg, uint8_t value)
// {
// 	gpio_clear(GPIOA, GPIO4);
// 	spi_send(SPI1, reg);
// 	spi_read(SPI1);
// 	spi_send(SPI1, value);
// 	spi_read(SPI1);
// 	gpio_set(GPIOA, GPIO4);
// }

static void setup_statusLED()
{
 	rcc_periph_clock_enable(LED_STATUS_RCC_PORT);
 	gpio_mode_setup(LED_STATUS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_STATUS_PIN);
 	gpio_set(LED_STATUS_PORT, LED_STATUS_PIN);
}

static void blink_statusLED()
{
 	gpio_toggle(LED_STATUS_PORT, LED_STATUS_PIN);
}

int main()
{
	system_millis = 0;
	setup_clock();
	setup_systick();
	USART::setup();
	setup_statusLED();

	uint32_t delay = 4000000;

	while(1)
	{
		//write_uart(2);
		printf("hi\n");
		blink_statusLED();
		//keep this for future testing

		msleep(1000);

		//for (uint32_t i = 0; i < delay; i++)
		//	__asm__("nop");
	}
	return 0;
}
