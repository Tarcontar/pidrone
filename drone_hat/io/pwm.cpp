#include "pwm.h"

#include <libopencm3/stm32/timer.h>

#include <stdint.h>

void PWM::init(volatile uint32_t *reg, uint32_t en, uint32_t timer_peripheral, uint32_t prescaler, uint32_t period);
{
	rcc_peripheral_enable_clock(reg, en);
	timer_reset(timer_peripheral);
	
	timer_set_mode(timer_peripheral, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_prescaler(timer_peripheral, prescaler);
	timer_set_repetition_counter(timer_peripheral, 0);
	timer_enable_preload(timer_peripheral);
	timer_continuous_mode(timer_peripheral);
	timer_set_period(timer_peripheral, period);
}

void PWM::init_output(uint32_t timer_peripheral, enum tim_oc_id oc_id, volatile uint32_t *gpio_reg, uint32_t gpio_en, uint32_t gpio_port, uint16_t gpio_pin)
{
	rcc_peripheral_enable_clock(gpio_reg, gpio_en);
	gpio_set_mode(gpio_port, GPIO_MODE_OUTPUT_50_MHZ,
		GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,
		gpio_pin);
		
	timer_disable_oc_output(timer_peripheral, oc_id);
	timer_set_oc_mode(timer_peripheral, oc_id, TIM_PCM_PWM1);
	timer_set_oc_value(timer_peripheral, oc_id, 0);
	timer_enable_oc_output(timer_peripheral, oc_id);
}
	
void PWM::start(uint32_t timer_peripheral)
{
	timer_enable_counter(timer_peripheral);
}

void PWM::set_pulse_witdh(uint32_t timer_peripheral, enum tim_oc_id oc_id, uint32_t pulse_width)
{
	timer_set_oc_value(timer_peripheral, oc_id, pulse_width);
}

