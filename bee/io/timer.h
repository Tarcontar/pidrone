#pragma once
#include <stdint.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

class Timer
{
public:
	Timer(rcc_periph_clken rcc_timer, uint32_t timer, uint8_t prescaler = 72, uint16_t period);

	void enableCH(rcc_periph_clken rcc_port, uint32_t channel, uint32_t port, uint32_t pin);

	void setCH(uint32_t channel, uint16_t value);
	
private:
	uint32_t m_timer;
};