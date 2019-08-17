#include "hat_pcb.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/systick.h>
#include <cstdio>
#include "io/usart.h"
#include "io/serial.h"
#include "io/sensors.h"
#include "sys/clock.h"

volatile uint32_t system_millis;
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
	Clock::setup();
	setup_systick();
	USART::setup();
	setup_statusLED();

	Sensors sensors;
	sensors.setup();

	while(1)
	{
		//write_uart(2);
		printf("hi\n");
		ser << "Wie geht es?";
		blink_statusLED();
		//keep this for future testing

		msleep(1000);
	}
	return 0;
}
