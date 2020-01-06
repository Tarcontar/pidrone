#include "hat_pcb.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <cstdio>
#include "sys/status.h"
#include "io/usart.h"
#include "io/serial.h"
#include "io/sensors.h"
#include "sys/clock.h"
#include "actuators/motors.h"

int main()
{
    Clock::setup();
    Status::setup();
    USART::setup();
    ser << "USART ready!\n";

    Sensors sensors;
    if (!sensors.setup()) return -1;
    sensors.update();

    Motors motors;
    motors.update(1100, 0, 0, 0);

    Status::update();
    ser << "Setup finished!\n";

    while (1)
    {
        sensors.update();
        motors.update(1100, 1100, 1100, 1100);

        //Clock::sleep(1000);
        //motors.update(1800, 1800, 1800, 1800);
        //Clock::sleep(1000);
    }
    return 0;
}

