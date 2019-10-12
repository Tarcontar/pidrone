#include "pwmreceiver.h"
#include "../sys/interrupt.h"
#include "../hat_pcb.h"
#include "../sys/systick.h"

#define FALLING 0
#define RISING 1

bool isRising(int idr) return (idr & 1) == 1;

void handleInterrupt(int exti, int idr, int id)
{
    if (isRising(idr)) rising_start[id] = SysTick::system_millis;
    else channel_length[id] = SysTick::system_millis - rising_start[id];
    exti_reset_request(exti);
}

PWMReceiver::PWMReceiver()
{
    Interrupt(REC_CHANNEL_1);
}

uint16_t PWMReceiver::getChannel(Channel channel)
{
    if (channel_pin[channel - 1] == -1) return -1;
    return channel_length[channel - 1];
}


