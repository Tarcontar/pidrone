#include "timer.h"

Timer::Timer(rcc_periph_clken rcc_timer, uint32_t timer, uint8_t prescaler, uint16_t period) : m_timer(timer)
{
	
	rcc_periph_clock_enable(rcc_timer);
	timer_reset(m_timer);
	timer_set_mode(m_timer, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1, TIM_CR1_DIR_UP);
	timer_set_prescaler(m_timer, 16); //16mhz to 1 mhz
	timer_set_period(m_timer, 1000000/PWM_REQ);
	timer_enable_break_main_output(m_timer);
	
	timer_enable_counter(TIM1);
}

void Timer::enableCH(rcc_periph_clken rcc_port, uint32_t channel, uint32_t port, uint32_t pin)
{
	rcc_periph_clock_enable(rcc_port);
	gpio_mode_setup(port, GPIO_MODE_AF, GPIO_PUPD_NONE, pin);
	gpio_set_af(port, GPIO_AF1, pin);
	timer_enable_oc_output(m_timer, channel);
	timer_set_oc_mode(m_timer, channel, TIM_OCM_PWM2);
}

void Timer::setCH(uint32_t channel, uint16_t value)
{
	timer_set_oc_value(m_timer, channel, value);
}