#include "timer.h"

#include <libopencm3/stm32/gpio.h>

Timer::Timer(rcc_periph_clken rcc_timer, uint32_t timer, uint8_t prescaler,
						 uint16_t period) : m_timer(timer)
{
	rcc_periph_clock_enable(rcc_timer);
	timer_reset(m_timer);
	timer_set_mode(m_timer, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_prescaler(m_timer, prescaler);
	timer_enable_preload(m_timer);
	timer_set_period(m_timer, period);

	timer_enable_counter(m_timer);
}

void Timer::enableChannel(rcc_periph_clken rcc_port, tim_oc_id channel, 
	uint32_t port, uint32_t pin)
{
	rcc_periph_clock_enable(rcc_port);
	gpio_set_mode(port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, pin);
	timer_enable_oc_output(m_timer, channel);
	timer_set_oc_value(m_timer, channel, 0);
	timer_set_oc_mode(m_timer, channel, TIM_OCM_PWM1);
}

void Timer::setChannel(tim_oc_id channel, uint16_t value)
{
	timer_set_oc_value(m_timer, channel, value);
}
