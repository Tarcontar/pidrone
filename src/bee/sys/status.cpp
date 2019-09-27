#include "status.h"
#include "../hat_pcb.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void Status::setup()
{
    rcc_periph_clock_enable(LED_STATUS_RCC_PORT);
	rcc_periph_clock_enable(LED_BLUE_RCC_PORT);
	rcc_periph_clock_enable(LED_ORANGE_RCC_PORT);

	gpio_mode_setup(LED_STATUS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_STATUS_PIN);
	gpio_mode_setup(LED_BLUE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_BLUE_PIN);
	gpio_mode_setup(LED_ORANGE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_ORANGE_PIN);

    gpio_set(LED_STATUS_PORT, LED_STATUS_PIN);
	gpio_set(LED_BLUE_PORT, LED_BLUE_PIN);
	gpio_set(LED_ORANGE_PORT, LED_ORANGE_PIN);
}

//TODO: use timer to toggle staus led every second
void Status::update()
{
  	gpio_toggle(LED_STATUS_PORT, LED_STATUS_PIN);
	gpio_toggle(LED_BLUE_PORT, LED_BLUE_PIN);
	gpio_toggle(LED_ORANGE_PORT, LED_ORANGE_PIN);
}
