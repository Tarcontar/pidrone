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
}

static void blink_statusLED()
{
	gpio_toggle(LED_STATUS_PORT, LED_STATUS_PIN);
}

int main()
{
	Clock::setup();
	SysTick::setup();
	USART::setup();
	setup_statusLED();

	ser << "Setup finished!\n";

	Sensors sensors;
	sensors.setup();

	while (1)
	{
		blink_statusLED();
		sensors.update();
		//keep this for future testing

		SysTick::sleep(100);
	}
	return 0;
}
