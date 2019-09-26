#include "hat_pcb.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <cstdio>
#include "io/usart.h"
#include "io/serial.h"
#include "io/sensors.h"
#include "sys/clock.h"
#include "sys/systick.h"

static void setup_statusLED()
{
	rcc_periph_clock_enable(LED_STATUS_RCC_PORT);
	gpio_mode_setup(LED_STATUS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_STATUS_PIN);
	gpio_set(LED_STATUS_PORT, LED_STATUS_PIN);

	rcc_periph_clock_enable(LED_BLUE_RCC_PORT);
	rcc_periph_clock_enable(LED_ORANGE_RCC_PORT);
	gpio_mode_setup(LED_BLUE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_BLUE_PIN);
	gpio_mode_setup(LED_ORANGE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_ORANGE_PIN);
	gpio_set(LED_BLUE_PORT, LED_BLUE_PIN);
	gpio_set(LED_ORANGE_PORT, LED_ORANGE_PIN);
	gpio_set(LED_STATUS_PORT, LED_STATUS_PIN);
}

static void blink_statusLED()
{
	gpio_toggle(LED_STATUS_PORT, LED_STATUS_PIN);
	gpio_toggle(LED_BLUE_PORT, LED_BLUE_PIN);
	gpio_toggle(LED_ORANGE_PORT, LED_ORANGE_PIN);
}

int main()
{
        setup_statusLED();
	Clock::setup();
	SysTick::setup();
	USART::setup();

	Sensors sensors;
	sensors.setup();

        blink_statusLED();
        ser << "Setup finished!\n";

	while (1)
	{
		//blink_statusLED();
		sensors.update();

		SysTick::sleep(1000);
	}
	return 0;
}

