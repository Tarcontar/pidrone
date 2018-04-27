#pragma once
#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>


class Timer
{
public:
	Timer(rcc_periph_clken rcc_timer, uint32_t timer, uint8_t prescaler = 72, uint16_t period = 20000);

	void enableCH(rcc_periph_clken rcc_port, tim_oc_id channel, uint32_t port, uint32_t pin);

	void setCH(tim_oc_id channel, uint16_t value);
	
private:
	uint32_t m_timer;
};
