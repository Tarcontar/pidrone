#pragma once
#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>

class Timer
{
public:
	Timer(rcc_periph_clken rcc_timer, uint32_t timer, uint8_t prescaler = 24,
				uint16_t period = 20000);

	void enableChannel(rcc_periph_clken rcc_port, tim_oc_id channel,
				uint32_t port, uint32_t pin);

	void setChannel(tim_oc_id channel, uint16_t value);
	
private:
	uint32_t m_timer;
};
