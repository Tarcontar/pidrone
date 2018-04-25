#pragma once

#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#define PWM_PRESCALE (8)
#define PWM_PERIOD (20000)

#define PWM_MAX (2050)
#define PWM_MIN (950)
#define PWM_NULL (1580)

class PWM
{
public:

	static void init(volatile uint32_t *reg = &RCC_APB1ENR, uint32_t en = RCC_APB1ENR_TIM2EN, uint32_t timer_peripheral = TIM2, uint32_t prescaler = PWM_PRESCALE, uint32_t period = PWM_PERIOD);
	static void init_output(uint32_t timer_peripheral = TIM2, enum tim_oc_id oc_id = TIM_OC2, volatile uint32_t *gpio_reg = &RCC_APB2ENR, uint32_t gpio_en = RCC_APB2ENR_IOPAEN, uint32_t gpio_port = GPIOA, uint16_t gpio_pin = GPIO_TIM2_CH2);
	
	static void start(uint32_t timer_peripheral = TIM2);
	static void set_pulse_width(uint32_t timer_peripheral = TIM2, enum tim_oc_id oc_id = TIM_OC2, uint32_t pulse_width = PWM_NULL);
	
private:
};