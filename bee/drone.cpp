#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

#include <string>

#include "hat_pcb.h"
#include "io/serial.h"
#include "actuators/motors.h"

//Serial ser(9600);
Motors *motors;

bool toggle = false;

void setup_status_led()
{
	rcc_periph_clock_enable(_LED_STATUS_RCC_PORT);
	gpio_clear(_LED_STATUS_PORT, _LED_STATUS_PIN);
	gpio_set_mode(_LED_STATUS_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, _LED_STATUS_PIN);
}

void sys_tick_handler(void)
{
	gpio_toggle(_LED_STATUS_PORT, _LED_STATUS_PIN);
	//ser << "loop" << std::endl;
	/*
	if (!toggle)
	{
		timer_set_oc_value(TIM3, TIM_OC1, 1700);
	}
	else
		timer_set_oc_value(TIM3, TIM_OC1, 300);
	toggle = !toggle;
	*/
}

int main(void)
{
	rcc_clock_setup_in_hse_16mhz_out_72mhz();//eig sind 8mhz verbaut aber geht nur mit 16?!?!

	setup_status_led();

	//ser << "System starting" << std::endl;

	//motors = new Motors();
	//motors->setupESCs();

	rcc_periph_clock_enable(RCC_TIM3);
	timer_reset(TIM3);
	timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
	timer_set_prescaler(TIM3, 36);
	//timer_set_repetition_counter(TIM3, 0);
	timer_enable_preload(TIM3);
	timer_set_period(TIM3, 20000);
	//timer_continuous_mode(TIM3);
	//timer_enable_counter(TIM3);

	rcc_periph_clock_enable(RCC_GPIOA);
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO6);
	timer_enable_oc_output(TIM3, TIM_OC1);
	timer_set_oc_value(TIM3, TIM_OC1, 0);
	timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM1);

	timer_enable_counter(TIM3);

	//systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	//systick_set_reload(3999999 * 8); //hack for 8mhz clock issue
	//systick_interrupt_enable();
	//XYsystick_counter_enable();

	while(1) 
	{
		uint32_t delay = 500000;
		timer_set_oc_value(TIM3, TIM_OC1, 1700);
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		timer_set_oc_value(TIM3, TIM_OC1, 300);
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		gpio_toggle(GPIOC, GPIO8);
	}

	return 0;
}
