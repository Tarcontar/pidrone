//#include "actuators/motors.h"
//#include "io/serial.h"
//#include "io/sensors.h"
//#include "sys/mcu.h"
//#include "sys/handler.h"
//#include "sys/status.h"
//#include <stdint.h>

#include "hat_pcb.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>

int main()
{
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(RCC_GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
	gpio_set(RCC_GPIOC, GPIO12);

	while(1)
	{
		//gpio_toggle(_LED_STATUS_PORT, _LED_STATUS_PIN);
		//Status::update();
		//keep this for future testing
		uint32_t delay = 45000;
		for (uint32_t i = 0; i < delay; i++)
			__asm__("NOP");
		//ser << "Hi from Serial! " << 545 << " " << 3.14 << " " << t << ser.endl;
	}
	return 0;
}
