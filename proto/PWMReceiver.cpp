#include "PWMReceiver.h"
#include <Arduino.h>
#include <PinChangeInterrupt.h>

volatile unsigned long rising_start[] = { 0, 0, 0, 0, 0, 0 };
volatile int channel_pin[6];
volatile long channel_length[6];

void processPWMPin(int pin)
{
  uint8_t trigger = getPinChangeInterruptTrigger(channel_pin[pin]);
  if (trigger == RISING)
  {
    rising_start[pin] = micros();
  }
  else if (trigger == FALLING)
  {
    channel_length[pin] = micros() - rising_start[pin];
  }
}


void onRising0()
{
  processPWMPin(0);
}

void onRising1()
{
  processPWMPin(1);
}

void onRising2()
{
  processPWMPin(2);
}

void onRising3()
{
  processPWMPin(3);
}

void onRising4()
{
  processPWMPin(4);
}

void onRising5()
{
  processPWMPin(5);
}

PWMReceiver::PWMReceiver()
{
 
}

void PWMReceiver::SetChannels(int ch_1, int ch_2, int ch_3, int ch_4, int ch_5, int ch_6)
{
  for (int i = 0; i < 6; i++) channel_pin[i] = -1;
  pinMode(ch_1, INPUT);
  channel_pin[0] = digitalPinToPinChangeInterrupt(ch_1);
  attachPinChangeInterrupt(channel_pin[0], onRising0, CHANGE);

  if (ch_2 != -1)
  {
    pinMode(ch_2, INPUT);
    channel_pin[1] = digitalPinToPinChangeInterrupt(ch_2);
    attachPinChangeInterrupt(channel_pin[1], onRising1, CHANGE);
  }
  if (ch_3 != -1)
  {
    pinMode(ch_3, INPUT);
    channel_pin[2] = digitalPinToPinChangeInterrupt(ch_3);
    attachPinChangeInterrupt(channel_pin[2], onRising2, CHANGE);
  }
  if (ch_4 != -1)
  {
    pinMode(ch_4, INPUT);
    channel_pin[3] = digitalPinToPinChangeInterrupt(ch_4);
    attachPinChangeInterrupt(channel_pin[3], onRising3, CHANGE);
  }
  if (ch_5 != -1)
  {
    pinMode(ch_5, INPUT);
    channel_pin[4] = digitalPinToPinChangeInterrupt(ch_5);
    attachPinChangeInterrupt(channel_pin[4], onRising4, CHANGE);
  }
  if (ch_6 != -1)
  {
    pinMode(ch_6, INPUT);
    channel_pin[5] = digitalPinToPinChangeInterrupt(ch_6);
    attachPinChangeInterrupt(channel_pin[5], &onRising5, CHANGE);
  }
}

long PWMReceiver::getChannel(int channel)
{
  if (channel_pin[channel - 1] == -1) return -1;
  return channel_length[channel - 1];
}


