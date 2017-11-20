#include "pwmreceiver.h"
#include <Arduino.h>
#define EI_ARDUINO_INTERRUPTED_PIN
#include <EnableInterrupt.h>

static unsigned long rising_start[] = { 0, 0, 0, 0, 0, 0 };
static uint8_t channel_pin[6];
static long channel_length[6];

#if defined(PCIE0)
#define digitalPinToPinChangeInterrupt(p) (digitalPinToPCICR(p) ? ((8 * (digitalPinToPCICRbit(p) - PCIE0)) + digitalPinToPCMSKbit(p)) : NOT_AN_INTERRUPT)
#elif defined(PCIE)
#define digitalPinToPinChangeInterrupt(p) (digitalPinToPCICR(p) ? ((8 * (digitalPinToPCICRbit(p) - PCIE)) + digitalPinToPCMSKbit(p)) : NOT_AN_INTERRUPT)
#else
#error MCU has no such a register
#endif

void rising()
{
 uint8_t pin = arduinoInterruptedPin;
 rising_start[pin] = micros();
}

void falling()
{
  uint8_t pin = arduinoInterruptedPin;
  channel_length[pin] = micros() - rising_start[pin];
}

PWMReceiver::PWMReceiver()
{
 
}

void PWMReceiver::SetChannels(const int8_t channels[6])
{
  for (uint8_t i = 0; i < 6; i++)
  {
    if(channels[i])
      continue;

    pinMode(channels[i], INPUT);
    channel_pin[i] = digitalPinToPinChangeInterrupt(channels[i]);
    enableInterrupt(channel_pin[i],rising,RISING);
    enableInterrupt(channel_pin[i],falling,FALLING);
  }
}

uint16_t PWMReceiver::getChannel(int8_t channel)
{
  if (channel_pin[channel - 1] == -1) 
    return -1;

  return channel_length[channel - 1];
}


