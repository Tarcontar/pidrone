#include "actuators/motors.h"
#include "io/serial.h"
#include "sys/mcu.h"
#include "sys/handler.h"
#include <stdint.h>
#include <string>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>

uint16_t read_adc_naiive(uint8_t channel)
{
	uint8_t channel_array[16];
	channel_array[0] = channel;
	adc_set_regular_sequence(ADC1, 1, channel_array);
	adc_start_conversion_direct(ADC1);
	while(!adc_eoc(ADC1));
	uint16_t reg16 = adc_read_regular(ADC1);
	return reg16;
}

int main(void)
{
	MCU::setup();

	Serial ser(9600);

	//Analog Read Testcode
	rcc_periph_clock_enable(RCC_ADC1);
	rcc_periph_clock_enable(RCC_GPIOA);

	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, GPIO0);
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, GPIO1);

	adc_power_off(ADC1);
	adc_disable_scan_mode(ADC1);
	adc_set_single_conversion_mode(ADC1);
	adc_disable_external_trigger_regular(ADC1);
	adc_set_right_aligned(ADC1);
	adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_28DOT5CYC);
	adc_power_on(ADC1);


	adc_reset_calibration(ADC1);
	adc_calibrate(ADC1);


	/*
	Handler::queryTask([ser]()
	{
		ser << "This is a test" << ser.endl;
	});
	*/

	while(1)
	{
		//keep this for future testing
		uint32_t delay = 500000;
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		ser.putc('H');
		ser.putc('\r');
		ser.putc('\n');
	}
	return 0;
}
