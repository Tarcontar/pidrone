#pragma once
#include <stdint.h>

class Clock
{
public:
    static void setup();
    static void sleep(uint32_t delay);
    static uint32_t millis();
};
