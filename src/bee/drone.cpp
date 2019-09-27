#include "hat_pcb.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <cstdio>
#include "sys/status.h"
#include "io/usart.h"
#include "io/serial.h"
#include "io/sensors.h"
#include "sys/clock.h"
#include "sys/systick.h"

int main()
{
    Clock::setup();
    SysTick::setup();
    Status::setup();
    USART::setup();
    ser << "USART ready!\n";

    Sensors sensors;
    sensors.setup();
    sensors.update();

    Status::update();
    ser << "Setup finished!\n";

    while (1)
    {
	//sensors.update();

	SysTick::sleep_mills(1000);
    }
    return 0;
}

